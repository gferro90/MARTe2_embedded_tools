/**
 * @file GpioDataSource.cpp
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
 * the class GpioDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#include "GpioDataSource.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

GpioDataSource::GpioDataSource() :
        MemoryDataSourceI() {
    //Initialisation already done by the tool !!!
    for (uint32 i = 0u; i < 10u; i++) {
        gpioHandlePtr[i] = NULL;
    }
    numberOfInputs = 0u;
}

GpioDataSource::~GpioDataSource() {
}

bool GpioDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString gpioId;
        if (!data.Read("NumberOfInputs", numberOfInputs)) {
            numberOfInputs = 0u;
            REPORT_ERROR(ErrorManagement::Information, "NumberOfInputs not specified. Assuming 0");
        }
    }

    return ret;
}

bool GpioDataSource::Synchronise() {

    for (uint32 i = 0u; i < numberOfSignals; i++) {
        if (i < numberOfInputs) {
            ((uint32*) memory)[i] = gpioHandlePtr[i]->IDR;
        } else {
            uint32 mask = ((uint32*) memory)[i];

            //reset first
            uint32 odr = gpioHandlePtr[i]->ODR;
            gpioHandlePtr[i]->BSRR = (odr << 16u);

            if (IS_GPIO_PIN(mask)) {
                gpioHandlePtr[i]->BSRR = mask;
            }

        }
    }
    return true;
}

bool GpioDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        ret = (numberOfSignals <= 10u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "GpioDataSource can handle up to 10 signals");
        }
    }
    if (ret) {
        for (uint32 i = 0u; i < numberOfSignals; i++) {
            StreamString gpioId;
            GetSignalName(i, gpioId);
            gpioHandlePtr[i] = (GPIO_TypeDef*) GetHandle(gpioId.Buffer());
            ret = (gpioHandlePtr != NULL);
            if (ret) {
                ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The GpioDataSource signal %d type must be uint32", i);
                }
            } else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid GpioDataSource handler %s", gpioId.Buffer());
            }
            if (ret) {
                uint8 nDims = 0u;
                ret = (GetSignalNumberOfDimensions(i, nDims));
                if (ret) {
                    ret = (nDims == 0u);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "The GpioDataSource signal %d must be a scalar", i);
                    }
                }
            }
        }
    }

    return ret;
}

const char8* GpioDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8 *brokerName = NULL;
    if (numberOfInputs == 0) {
        brokerName = "MemoryMapSynchronisedOutputBroker";
    } else {
        if (direction == OutputSignals) {
            brokerName = "MemoryMapOutputBroker";
        } else {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
    }
    return brokerName;
}

bool GpioDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    return true;
}

CLASS_REGISTER(GpioDataSource, "1.0")
