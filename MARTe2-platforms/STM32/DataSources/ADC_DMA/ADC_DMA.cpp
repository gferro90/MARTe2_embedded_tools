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

#include "ADC_DMA.h"
#include "AdvancedErrorManagement.h"
#include "ADC_DMAReader.h"
#include "HandleDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



ADC_DMA::ADC_DMA() :
        DataSourceI() {
//Initialisation already done by the tool !!!
    ADC_DMAHandlePtr = NULL;
    maxVal = 0u;
    minVal = 0u;
}

ADC_DMA::~ADC_DMA() {

    HAL_ADC_Stop_DMA(ADC_DMAHandlePtr);
}

bool ADC_DMA::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    /*while(1) {
     const char8 * name=GetClassProperties()->GetName();
     REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError, "ahah %s",name );
     Sleep::MSec(100);
     }*/
    if (ret) {
        StreamString adcId;
        ret = data.Read("AdcIdentifier", adcId);
        if (ret) {
            ADC_DMAHandlePtr = (ADC_HandleTypeDef *)GetHandle(adcId.Buffer());
            ret=(ADC_DMAHandlePtr!=NULL);
        }
        else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "ADC not specified");
        }

        if (ret) {
            AnyType channelsAT = data.GetType("Channels");
            ret = !channelsAT.IsVoid();
            if (ret) {
                uint32 numberOfChannels = channelsAT.GetNumberOfElements(0u);
                Vector < uint32 > channelsVec(numberOfChannels);
                ret = data.Read("Channels", channelsVec);
                if (ret) {
                    for (uint32 i = 0u; i < numberOfChannels && ret; i++) {
                        ret = (channelsVec[i] < 16);
                        if (!ret) {
                            REPORT_ERROR(ErrorManagement::InitialisationError, "The Channel number must be < 16");
                        }
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to read Channels field");
                }
            }
            else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Unable to read Channels field");
            }

        }
        if (ret) {
            if (!data.Read("MaxValue", maxVal)) {
                maxVal = 4095;
                REPORT_ERROR(ErrorManagement::Warning, "Max Value not set: impose 4096 by default");
            }
            if (!data.Read("MinValue", minVal)) {
                minVal = 0;
                REPORT_ERROR(ErrorManagement::Warning, "Min Value not set: impose 0 by default");
            }
        }
    }
    return ret;
}

bool ADC_DMA::Synchronise() {
    return true;
}

bool ADC_DMA::AllocateMemory() {
    return true;
}

uint32 ADC_DMA::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool ADC_DMA::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();
        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {
            ret = (GetSignalType(i) == TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The ADC_DMA signal type has to be uint32");
            }
        }
    }
    return ret;
}

bool ADC_DMA::GetSignalMemoryBuffer(const uint32 signalIdx,
                                    const uint32 bufferIdx,
                                    void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *ADC_DMA::GetBrokerName(StructuredDataI &data,
                                    const SignalDirection direction) {
    if (direction == InputSignals) {
        return "ADC_DMAReader";
    }

    return "";
}

bool ADC_DMA::GetInputBrokers(ReferenceContainer &inputBrokers,
                              const char8* const functionName,
                              void * const gamMemPtr) {
    ReferenceT<ADC_DMAReader> broker("ADC_DMAReader");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool ADC_DMA::GetOutputBrokers(ReferenceContainer &outputBrokers,
                               const char8* const functionName,
                               void * const gamMemPtr) {
    // write not allowed
    return false;
}

bool ADC_DMA::PrepareNextState(const MARTe::char8 * const currentStateName,
                               const MARTe::char8 * const nextStateName) {
    return true;
}

ADC_HandleTypeDef * ADC_DMA::GetHwHandle() {
//todo in the main map into an array the handles of the defined uarts
//define this function
    return ADC_DMAHandlePtr;
}

uint32 ADC_DMA::GetMaxVal() const {
    return maxVal;
}
uint32 ADC_DMA::GetMinVal() const {
    return minVal;
}

CLASS_REGISTER(ADC_DMA, "1.0")
