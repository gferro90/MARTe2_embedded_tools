/**
 * @file PwmDataSource.cpp
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
 * the class PwmDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#include "PwmDataSource.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

static const uint32 channelNames[] = { TIM_CHANNEL_1, TIM_CHANNEL_2, TIM_CHANNEL_3, TIM_CHANNEL_4 };

PwmDataSource::PwmDataSource() :
        MemoryDataSourceI() {
    //Initialisation already done by the tool !!!
    pwmHandle = NULL;

    startVal=0u;
}

PwmDataSource::~PwmDataSource() {
}

bool PwmDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString pwmId;
        ret = data.Read("Identifier", pwmId);
        if (ret) {
            pwmHandle = (TIM_HandleTypeDef*) GetHandle(pwmId.Buffer());
            ret = (pwmHandle != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid PwmDataSource handler %s", pwmId.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No PwmDataSource Identifier specified");
        }

        if (ret) {
            //get the zero duty cycle
            if (!data.Read("StartVal", startVal)) {
                startVal = 0u;
                REPORT_ERROR(ErrorManagement::Warning, "Undefined ZeroVal field. Set to default %d", startVal);
            }
        }
    }

    return ret;
}

bool PwmDataSource::Synchronise() {

    uint32 *dutyCycles = (uint32*) memory;

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        __HAL_TIM_SET_COMPARE(pwmHandle, channelNames[i], dutyCycles[i]);
    }

    return true;
}

bool PwmDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The PwmDataSource signal type must be uint32");
            }
            if (ret) {
                uint8 nDims = 0u;
                ret = (GetSignalNumberOfDimensions(0, nDims));
                if (ret) {
                    ret = (nDims == 0u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "The PwmDataSource signal must be a scalar");
                    }
                }
            }
        }
    }

    return ret;
}

const char8* PwmDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == OutputSignals) {
        return "MemoryMapSynchronisedOutputBroker";
    }

    return "";
}

bool PwmDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    for (uint32 i = 0u; i < numberOfSignals; i++) {
        __HAL_TIM_SET_COMPARE(pwmHandle, channelNames[i], startVal);
        HAL_TIM_PWM_Start(pwmHandle, channelNames[i]);
    }
    return true;
}

TIM_HandleTypeDef* PwmDataSource::GetHwHandle() {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    return pwmHandle;
}

CLASS_REGISTER(PwmDataSource, "1.0")
