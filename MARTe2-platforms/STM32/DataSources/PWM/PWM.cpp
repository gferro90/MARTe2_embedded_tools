/*
 * PWM.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "PWM.h"
#include "AdvancedErrorManagement.h"
#include "PWMReader.h"
#include "PWMWriter.h"
#include "HandleDatabase.h"

//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters



static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };

PWM::PWM() :
        DataSourceI() {
    //Initialisation already done by the tool !!!
    channel = 0u;
    pwmHandlePtr = NULL;
    zeroVal = 0u;
}

PWM::~PWM() {

    HAL_TIM_PWM_Stop(pwmHandlePtr, channelNames[channel]);
}

bool PWM::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        StreamString pwmId;
        ret = data.Read("TimIdentifier", pwmId);
        if (ret) {
            pwmHandlePtr = (TIM_HandleTypeDef *) GetHandle(pwmId.Buffer());
            ret = (pwmHandlePtr != NULL);
        }
        else {
            REPORT_ERROR(ErrorManagement::Warning, "No PWM specified");
        }

        if (ret) {
            //get the zero duty cycle
            zeroVal = 0u;
            if (!data.Read("ZeroVal", zeroVal)) {
                REPORT_ERROR(ErrorManagement::Warning, "Undefined ZeroVal field... we set it to zero");
                zeroVal = 0u;
            }
        }

        if (ret) {
            ret = data.Read("Channel", channel);
            if (ret) {
                ret = (channel < 4);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The Channel number must be < 4");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to read Channels field");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The handle is not valid");
        }
    }
    return ret;
}

bool PWM::Synchronise() {
    return true;
}

bool PWM::AllocateMemory() {
    return true;
}

uint32 PWM::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool PWM::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();

        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {

            ret = ((GetSignalType(i).type == SignedInteger) ||(GetSignalType(i).type == UnsignedInteger));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The pwm signal type has to be uint32");
            }
        }
    }

    if (ret) {
        if (IS_TIM_CCX_INSTANCE(pwmHandlePtr->Instance, channelNames[channel])) {

            __HAL_TIM_SET_COMPARE(pwmHandlePtr, channelNames[channel], zeroVal);
            HAL_TIM_PWM_Start(pwmHandlePtr, channelNames[channel]);

        }
        else {
            ret = false;
            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "The Channel %d must be initialised", channel);
        }
    }

    return ret;
}

bool PWM::GetSignalMemoryBuffer(const uint32 signalIdx,
                                const uint32 bufferIdx,
                                void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *PWM::GetBrokerName(StructuredDataI &data,
                                const SignalDirection direction) {
    if (direction == InputSignals) {
        return "PWMReader";
    }
    if (direction == OutputSignals) {
        return "PWMWriter";
    }
    return "";
}

bool PWM::GetInputBrokers(ReferenceContainer &inputBrokers,
                          const char8* const functionName,
                          void * const gamMemPtr) {
    ReferenceT<PWMReader> broker("PWMReader");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool PWM::GetOutputBrokers(ReferenceContainer &outputBrokers,
                           const char8* const functionName,
                           void * const gamMemPtr) {
    ReferenceT<PWMWriter> broker("PWMWriter");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool PWM::PrepareNextState(const MARTe::char8 * const currentStateName,
                           const MARTe::char8 * const nextStateName) {
    return true;
}

TIM_HandleTypeDef * PWM::GetHandle() {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    return pwmHandlePtr;
}

uint32 PWM::GetChannel() const {
    return channel;
}

CLASS_REGISTER(PWM, "1.0")
