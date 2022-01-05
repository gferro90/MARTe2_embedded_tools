/*
 * PWM_opt.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "PWM_opt.h"
#include "AdvancedErrorManagement.h"
#include "PWMReader_opt.h"
#include "PWMWriter_opt.h"
#include "HandleDatabase.h"
//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters

static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };

PWM_opt::PWM_opt() :
        DataSourceI() {
    //Initialisation already done by the tool !!!
    channel = NULL;
    pwmHandlePtr = NULL;
    zeroVal = NULL;
    numberOfElements = 0u;
}

PWM_opt::~PWM_opt() {

    for (uint32 i = 0u; i < numberOfElements; i++) {
        HAL_TIM_PWM_Stop(pwmHandlePtr[i], channelNames[channel[i]]);
    }

    if (channel != NULL) {
        delete[] channel;
    }

    if (zeroVal != NULL) {
        delete[] zeroVal;
    }
    if (pwmHandlePtr != NULL) {
        delete[] pwmHandlePtr;
    }

}

bool PWM_opt::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {

        //read timers
        AnyType timAT = data.GetType("TimIdentifiers");
        ret = !timAT.IsVoid();

        if (ret) {
            numberOfElements = timAT.GetNumberOfElements(0u);
            Vector < uint8 > timNums(numberOfElements);
            ret = data.Read("TimIdentifiers", timNums);
            if (ret) {
                pwmHandlePtr = new TIM_HandleTypeDef *[numberOfElements];
                char8 timIdArr[] = { 'T', 'I', 'M', 0, 0, 0 };

                for (uint8 i = 0u; i < numberOfElements && ret; i++) {
                    uint8 index = 3u;
                    for (uint8 k = 0u; k < 3; k++) {
                        timIdArr[index + k] = 0;
                    }

                    while (timNums[i] > 0u) {
                        if (index > 4u) {
                            ret = false;
                            REPORT_ERROR(ErrorManagement::InitialisationError, "The timer number can not exceed 2 digits");
                            break;
                        }
                        for (uint8 k = index; k > 3u; k--) {
                            timIdArr[k] = timIdArr[k - 1];
                        }
                        timIdArr[3] = (timNums[i] % 10)+'0';
                        timNums[i] /= 10;
                        index++;
                    }
                    if (ret) {
                        pwmHandlePtr[i] = (TIM_HandleTypeDef *) GetHandle(timIdArr);
                        ret=(pwmHandlePtr[i]!=NULL);
                        if(!ret){
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Timer %s not found", timIdArr);
                        }
                    }
                }
            }
        }

        //read channels
        if (ret) {
            channel = new uint8[numberOfElements];
            Vector < uint8 > channelVec(channel, numberOfElements);
            ret = data.Read("Channels", channelVec);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Channels must be specified");
            }
        }

        if (ret) {
            //get the zero duty cycle
            zeroVal = new uint32[numberOfElements];
            if (!data.Read("ZeroVals", zeroVal)) {
                REPORT_ERROR(ErrorManagement::Warning, "Undefined ZeroVal field... we set it to zero");
                for (uint8 i = 0u; i < numberOfElements; i++) {
                    zeroVal[i] = 0u;
                }
            }
        }

    }
    return ret;
}

bool PWM_opt::Synchronise() {
    return true;
}

bool PWM_opt::AllocateMemory() {
    return true;
}

uint32 PWM_opt::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool PWM_opt::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();

        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {

            ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The pwm signals type has to be uint32");
            }

            if (ret) {
                uint32 numberOfElements_rec = 0u;
                ret = GetSignalNumberOfElements(i, numberOfElements_rec);
                if (ret) {
                    ret = numberOfElements_rec == numberOfElements;
                }
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "All signals must have the number of elements equal to the number of pwms");
                }
            }
        }
    }

    if (ret) {
        for (uint8 i = 0u; i < numberOfElements; i++) {
            if (IS_TIM_CCX_INSTANCE(pwmHandlePtr[i]->Instance, channelNames[channel[i]])) {

                __HAL_TIM_SET_COMPARE(pwmHandlePtr[i], channelNames[channel[i]], zeroVal[i]);
                HAL_TIM_PWM_Start(pwmHandlePtr[i], channelNames[channel[i]]);

            }
            else {
                ret = false;
                REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The Channel %d is not available for the respective tim", channel[i]);
            }
        }
    }

    return ret;
}

bool PWM_opt::GetSignalMemoryBuffer(const uint32 signalIdx,
                                    const uint32 bufferIdx,
                                    void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *PWM_opt::GetBrokerName(StructuredDataI &data,
                                    const SignalDirection direction) {
    if (direction == InputSignals) {
        return "PWMReader_opt";
    }
    if (direction == OutputSignals) {
        return "PWMWriter_opt";
    }
    return "";
}

bool PWM_opt::GetInputBrokers(ReferenceContainer &inputBrokers,
                              const char8* const functionName,
                              void * const gamMemPtr) {
    ReferenceT<PWMReader_opt> broker("PWMReader_opt");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool PWM_opt::GetOutputBrokers(ReferenceContainer &outputBrokers,
                               const char8* const functionName,
                               void * const gamMemPtr) {
    ReferenceT<PWMWriter_opt> broker("PWMWriter_opt");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool PWM_opt::PrepareNextState(const MARTe::char8 * const currentStateName,
                               const MARTe::char8 * const nextStateName) {
    return true;
}

void PWM_opt::Write(uint32 * values) {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    for (uint8 i = 0u; i < numberOfElements; i++) {
        __HAL_TIM_SET_COMPARE(pwmHandlePtr[i], channelNames[channel[i]], values[i]);
    }
}

void PWM_opt::Read(uint32 * values) {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    for (uint8 i = 0u; i < numberOfElements; i++) {
        values[i] = __HAL_TIM_GET_COMPARE(pwmHandlePtr[i], channelNames[channel[i]]);
    }
}

CLASS_REGISTER(PWM_opt, "1.0")
