/**
 * @file GpioReader.cpp
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
 * the class GpioReader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#include "GpioReader.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

GpioReader::GpioReader() :
        MemoryDataSourceI() {
    //Initialisation already done by the tool !!!
    gpioHandlePtr = NULL;
}

GpioReader::~GpioReader() {
}

bool GpioReader::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString gpioId;
        ret = data.Read("Identifier", gpioId);
        if (ret) {
            gpioHandlePtr = (GPIO_TypeDef*) GetHandle(gpioId.Buffer());
            ret = (gpioHandlePtr != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid GpioReader handler %s", gpioId.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No GpioReader specified");
        }
    }

    return ret;
}

bool GpioReader::Synchronise() {
    *(uint32*) memory = (gpioHandlePtr->IDR);

    return true;
}

bool GpioReader::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);

    if (ret) {
        ret = (numberOfSignals == 1u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The GpioReader must contain only one uint32 signal");
        }
    }
    if (ret) {
        ret = (GetSignalType(0) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The GpioReader signal type must be uint32");
        }
        if (ret) {
            uint8 nDims = 0u;
            ret = (GetSignalNumberOfDimensions(0, nDims));
            if (ret) {
                ret = (nDims == 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The GpioReader signal must be a scalar");
                }
            }
        }
    }

    return ret;
}

const char8* GpioReader::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    }

    return "";
}

bool GpioReader::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    return true;
}

GPIO_TypeDef* GpioReader::GetHwHandle() {
    //todo in the main map into an array the handles of the defined uarts
    //define this function
    return gpioHandlePtr;
}

CLASS_REGISTER(GpioReader, "1.0")
