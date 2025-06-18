/**
 * @file ADCDMA.cpp
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
 * the class ADCDMA (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdcDmaDataSource.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

AdcDmaDataSource::AdcDmaDataSource() :
        MemoryDataSourceI() {
//Initialisation already done by the tool !!!
    AdcDmaDataSourceHandlePtr = NULL;
    started = false;
}

AdcDmaDataSource::~AdcDmaDataSource() {
    if (started) {
        HAL_ADC_Stop_DMA(AdcDmaDataSourceHandlePtr);
    }
}

bool AdcDmaDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString adcId;
        ret = data.Read("Identifier", adcId);
        if (ret) {
            AdcDmaDataSourceHandlePtr = (ADC_HandleTypeDef*) GetHandle(adcId.Buffer());
            ret = (AdcDmaDataSourceHandlePtr != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "ADC handler is not valid");
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "ADC not specified");
        }
    }
    return ret;
}

bool AdcDmaDataSource::Synchronise() {
    return true;
}

bool AdcDmaDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();
        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {
            ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
            if (ret) {
                uint32 numberOfElements;
                uint8 numberOfDimensions;
                GetSignalNumberOfElements(i, numberOfElements);
                GetSignalNumberOfDimensions(i, numberOfDimensions);
                ret = (numberOfElements == 1u) && (numberOfDimensions == 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The AdcDmaDataSource signal has to be scalar");
                }
            } else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The AdcDmaDataSource signal type has to be uint32");
            }
        }
    }
    return ret;
}

const char8* AdcDmaDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapInputBroker";
    }

    return "";
}

bool AdcDmaDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    bool ret = true;
    if (!started) {
        ret = (HAL_ADC_Start_DMA(AdcDmaDataSourceHandlePtr, (uint32_t*) memory, numberOfSignals) == HAL_OK);
        started = true;
    }
    return ret;
}

CLASS_REGISTER(AdcDmaDataSource, "1.0")
