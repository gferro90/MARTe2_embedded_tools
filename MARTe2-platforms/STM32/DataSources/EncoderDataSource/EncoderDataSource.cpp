/**
 * @file EncoderDataSource.cpp
 * @brief Source file for class ADCDMA
 * @date 28/set/2016
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class EncoderDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#include "EncoderDataSource.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

EncoderDataSource::EncoderDataSource() :
        DataSourceI() {
    //Initialisation already done by the tool !!!
    encoderHandle = NULL;
    lastCounter = 0u;
    invert = 0u;
    counter = 0;
    reset = 0u;
}

EncoderDataSource::~EncoderDataSource() {
}

bool EncoderDataSource::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        StreamString encoderId;
        ret = data.Read("Identifier", encoderId);
        if (ret) {
            encoderHandle = (TIM_HandleTypeDef*) GetHandle(encoderId.Buffer());
            ret = (encoderHandle != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid EncoderDataSource handler %s", encoderId.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No EncoderDataSource Identifier specified");
        }

        if (ret) {
            //get the zero duty cycle
            if (!data.Read("Invert", invert)) {
                invert = 0u;
                REPORT_ERROR(ErrorManagement::Information, "Invert not defined. Set to default %d", invert);
            }
        }
    }

    return ret;
}

bool EncoderDataSource::Synchronise() {
    //first signal is reset

    if (reset > 0u) {
        __HAL_TIM_SET_COUNTER(encoderHandle, 0);
        counter = 0;
    }

    uint16 innerCounter = __HAL_TIM_GET_COUNTER(encoderHandle);

    int32 delta = (int32)((int16)(innerCounter - lastCounter));
    lastCounter = innerCounter;

    counter += (invert > 0u) ? (-delta) : (delta);
    //REPORT_ERROR(ErrorManagement::Information, "Counter %d", counter);

    return true;
}

bool EncoderDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        ret = (numberOfSignals == 2u);
    }
    if (ret) {
        ret = (GetSignalType(0u) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint8"));
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The EncoderDataSource reset signal type must be uint8");
        }
    }
    if (ret) {
        ret = (GetSignalType(1u) == TypeDescriptor::GetTypeDescriptorFromTypeName("int32"));
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The EncoderDataSource counter signal type must be int32");
        }
    }
    if (ret) {
        for (uint32 i = 0u; (i < numberOfSignals) && ret; i++) {

            uint8 nDims = 0u;
            ret = (GetSignalNumberOfDimensions(i, nDims));
            if (ret) {
                ret = (nDims == 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The EncoderDataSource signal[%d] must be a scalar", i);
                }
            }
        }
    }

    return ret;
}

const char8* EncoderDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    } else {
        return "MemoryMapOutputBroker";
    }

    return "";
}



bool EncoderDataSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ret = true;
    if (signalIdx == 0u) {
        signalAddress = &reset;
    } else if (signalIdx == 1u) {
        signalAddress = &counter;
    } else {
        ret = false;
    }
    return ret;
}

bool EncoderDataSource::AllocateMemory(){
    return true;
}


bool EncoderDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    counter = 0;

    __HAL_TIM_SET_COUNTER(encoderHandle, 0);
    HAL_TIM_Encoder_Start(encoderHandle, TIM_CHANNEL_ALL);

    return true;
}

TIM_HandleTypeDef* EncoderDataSource::GetHwHandle() {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    return encoderHandle;
}

CLASS_REGISTER(EncoderDataSource, "1.0")
