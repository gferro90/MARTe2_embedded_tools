/*
 * EncodersCounter_opt.cpp
 *
 *  Created on: 31/ago/2016
 *      Author: pc
 */

#include "EncodersCounter_opt.h"
#include "AdvancedErrorManagement.h"
#include "EncodersReader_opt.h"
#include "EncodersWriter_opt.h"
#include "HandleDatabase.h"

//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters

EncodersCounter_opt::EncodersCounter_opt() :
        DataSourceI() {
    //Initialisation already done by the tool !!!
    encoderHandlePtr = NULL;
    resetEncoder = 0u;
    numberOfElements = 0u;
}

EncodersCounter_opt::~EncodersCounter_opt() {

    for (uint8 i = 0u; i < numberOfElements; i++) {
        HAL_TIM_Encoder_Stop(encoderHandlePtr[i], TIM_CHANNEL_ALL);
    }

    if (encoderHandlePtr != NULL) {
        delete[] encoderHandlePtr;
    }

}

bool EncodersCounter_opt::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);

    if (ret) {
        if (!data.Read("ResetEncoder", resetEncoder)) {
            resetEncoder = 0u;
        }

        //read timers
        AnyType timAT = data.GetType("TimIdentifiers");
        ret = !timAT.IsVoid();

        if (ret) {
            numberOfElements = timAT.GetNumberOfElements(0u);
            Vector < uint8 > timNums(numberOfElements);
            ret = data.Read("TimIdentifiers", timNums);
            if (ret) {
                encoderHandlePtr = new TIM_HandleTypeDef *[numberOfElements];
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
                        timIdArr[3] = (timNums[i] % 10) + '0';
                        timNums[i] /= 10;
                        index++;
                    }
                    if (ret) {
                        encoderHandlePtr[i] = (TIM_HandleTypeDef *) GetHandle(timIdArr);
                        ret = (encoderHandlePtr[i] != NULL);
                        if (!ret) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "Timer %s not found", timIdArr);
                        }
                    }
                }
            }
        }

    }

    return ret;
}

bool EncodersCounter_opt::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();

        for (uint8 i = 0u; (i < numberOfSignals) && ret; i++) {
            if (ret) {
                ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The encoder signal type has to be uint32");
                }

                if (ret) {
                    uint32 numberOfElements_rec = 0u;
                    ret = GetSignalNumberOfElements(i, numberOfElements_rec);
                    if (ret) {
                        ret = numberOfElements_rec == numberOfElements;
                    }
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "All signals must have the number of elements equal to the number of timers");
                    }
                }

            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The encoder data source allows one signal only");
            }
        }
        if (ret) {
            for (uint8 i = 0u; i < numberOfElements; i++) {
                HAL_TIM_Encoder_Start(encoderHandlePtr[i], TIM_CHANNEL_ALL);
            }

        }
    }

    return ret;
}

bool EncodersCounter_opt::PrepareNextState(const char8 * const currentStateName,
                                           const char8 * const nextStateName) {
    if (resetEncoder) {
        for (uint8 i = 0u; i < numberOfElements; i++) {
            __HAL_TIM_SET_COUNTER(encoderHandlePtr[i], 0);
        }
    }
    return true;
}

bool EncodersCounter_opt::Synchronise() {
    return true;
}

bool EncodersCounter_opt::AllocateMemory() {
    return true;
}

uint32 EncodersCounter_opt::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool EncodersCounter_opt::GetSignalMemoryBuffer(const uint32 signalIdx,
                                                const uint32 bufferIdx,
                                                void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *EncodersCounter_opt::GetBrokerName(StructuredDataI &data,
                                                const SignalDirection direction) {
    if (direction == InputSignals) {
        return "EncodersReader_opt";
    }
    if (direction == OutputSignals) {
        return "EncodersWriter_opt";
    }
    return "";
}

bool EncodersCounter_opt::GetInputBrokers(ReferenceContainer &inputBrokers,
                                          const char8* const functionName,
                                          void * const gamMemPtr) {
    ReferenceT<EncodersReader_opt> broker("EncodersReader_opt");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool EncodersCounter_opt::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                           const char8* const functionName,
                                           void * const gamMemPtr) {
    ReferenceT<EncodersWriter_opt> broker("EncodersWriter_opt");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

void EncodersCounter_opt::Write(int32 * values) {
    for (uint8 i = 0u; i < numberOfElements; i++) {

        __HAL_TIM_SET_COUNTER(encoderHandlePtr[i], values[i]);
    }
}

void EncodersCounter_opt::Read(int32 * values) {
    for (uint8 i = 0u; i < numberOfElements; i++) {
        if (values[i]== -1) {
            __HAL_TIM_SET_COUNTER(encoderHandlePtr[i], 0);
        }
        values[i] = __HAL_TIM_GET_COUNTER(encoderHandlePtr[i]);
    }

}

CLASS_REGISTER(EncodersCounter_opt, "1.0")
