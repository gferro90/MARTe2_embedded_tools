/**
 * @file AdcDmaDataSource.cpp
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
 * the class AdcDmaDataSource (public, protected, and private). Be aware that some
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
static uint8 _adc_dma_numberOfChannels = 0u;
static uint16 _adc_dma_numberOfElements = 0u;
static uint16 *_adc_dma_dsBufferCh = NULL;
static uint8 _adc_dma_readIdx = 0u;
static uint8 _adc_dma_numberOfBuffers = 0u;
static uint32 _adc_dma_sizeCh = 0u;
static uint16 *_adc_dma_adcBufferCh = NULL;

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {

    for (uint8 i = 0u; i < _adc_dma_numberOfChannels; i++) {
        for (uint16 j = 0u; j < _adc_dma_numberOfElements; j++) {
            uint32 startIdx = (i * _adc_dma_numberOfBuffers * _adc_dma_numberOfElements) + (_adc_dma_readIdx * _adc_dma_numberOfElements);
            _adc_dma_dsBufferCh[startIdx + j] = _adc_dma_adcBufferCh[(j * _adc_dma_numberOfChannels) + i];
        }
    }
    _adc_dma_readIdx++;
    _adc_dma_readIdx %= _adc_dma_numberOfBuffers;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {

    for (uint8 i = 0u; i < _adc_dma_numberOfChannels; i++) {
        for (uint16 j = 0u; j < _adc_dma_numberOfElements; j++) {
            uint32 startIdx = (i * _adc_dma_numberOfBuffers * _adc_dma_numberOfElements) + (_adc_dma_readIdx * _adc_dma_numberOfElements);
            _adc_dma_dsBufferCh[startIdx + j] = _adc_dma_adcBufferCh[_adc_dma_sizeCh + ((j * _adc_dma_numberOfChannels) + i)];
        }
    }
    _adc_dma_readIdx++;
    _adc_dma_readIdx %= _adc_dma_numberOfBuffers;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

AdcDmaDataSource::AdcDmaDataSource() :
        MemoryDataSourceI() {
//Initialisation already done by the tool !!!
    adcDmaDataSourceHandlePtr = NULL;
    started = false;
    nSignalElementsLocal = 0u;
    writtenIdx = 0u;
}

AdcDmaDataSource::~AdcDmaDataSource() {
    if (started) {
        HAL_ADC_Stop_DMA(adcDmaDataSourceHandlePtr);
    }
}

bool AdcDmaDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString adcId;
        ret = data.Read("Identifier", adcId);
        if (ret) {
            adcDmaDataSourceHandlePtr = (ADC_HandleTypeDef*) GetHandle(adcId.Buffer());
            ret = (adcDmaDataSourceHandlePtr != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "ADC handler is not valid");
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "ADC not specified");
        }
        if (ret) {
            StreamString timerId;
            ret = data.Read("TriggerTimer", timerId);
            if (ret) {
                triggerTimerHandlePtr = (TIM_HandleTypeDef*) GetHandle(timerId.Buffer());
                ret = (triggerTimerHandlePtr != NULL);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Trigger timer handler is not valid");
                }
            } else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Trigger timer not specified");
            }
        }
        if (ret) {
            float32 convFreq;
            if (data.Read("ConversionFrequency", convFreq)) {
                uint32 preload = (uint32)((1e6) / convFreq);
                float32 realFreq = (1e6 / preload);
                preload--;
                REPORT_ERROR(ErrorManagement::Information, "ConversionFrequency %f approximated to %f: preload=%d", convFreq, realFreq, preload);

                triggerTimerHandlePtr->Init.Period = preload;
                ret = (HAL_TIM_Base_Init(triggerTimerHandlePtr) == HAL_OK);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to set preload %d to trigger timer", preload);
                }
            }
            if (numberOfBuffers == 1u) {
                REPORT_ERROR(ErrorManagement::Warning, "It is recommended NumberOfBuffers >= 2");
            }
        }
    }
    return ret;
}

bool AdcDmaDataSource::GetInputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {
    bool ret = true;
    offset = (writtenIdx * nSignalElementsLocal * sizeof(uint16));

    return ret;
}

bool AdcDmaDataSource::Synchronise() {
    uint8 nextRead = (_adc_dma_readIdx + 1u) % ((uint8) numberOfBuffers);
    if ((writtenIdx != _adc_dma_readIdx) && (writtenIdx != nextRead)) {
        writtenIdx++;
        writtenIdx %= (uint8) numberOfBuffers;
    }
    return true;
}

bool AdcDmaDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();
        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {
            ret = (GetSignalType(i) == UnsignedInteger16Bit);
            if (ret) {
                uint32 numberOfElements;
                GetSignalNumberOfElements(i, numberOfElements);
                if (nSignalElementsLocal == 0u) {
                    nSignalElementsLocal = numberOfElements;
                } else {
                    ret = (nSignalElementsLocal == numberOfElements);
                    if (!ret) {
                        REPORT_ERROR(ErrorManagement::InitialisationError, "All the ADC channels must have the same NumberOfElements");
                    }
                }
            } else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The AdcDmaDataSource signal type has to be uint16");
            }
        }
    }
    return ret;
}

const char8* AdcDmaDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedMultiBufferInputBroker";
    }

    return "";
}

bool AdcDmaDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {

    bool ret = true;
    if (!started) {
        //timer triggers DAC conversion
        ret = (HAL_TIM_Base_Start_IT(triggerTimerHandlePtr) == HAL_OK);
        if (ret) {
            _adc_dma_numberOfBuffers = (uint8) numberOfBuffers;
            _adc_dma_sizeCh = (numberOfSignals * nSignalElementsLocal);
            _adc_dma_numberOfChannels = numberOfSignals;
            _adc_dma_numberOfElements = nSignalElementsLocal;
            _adc_dma_adcBufferCh = (uint16*) HeapManager::Malloc(2 * _adc_dma_sizeCh * sizeof(uint16));
            _adc_dma_dsBufferCh = (uint16*) (memory);

            ret = (HAL_ADC_Start_DMA(adcDmaDataSourceHandlePtr, (uint32_t*) _adc_dma_adcBufferCh, 2 * _adc_dma_sizeCh) == HAL_OK);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed to start ADC DMA");
            }
        } else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to start trigger timer");
        }
        started = true;
    }
    return ret;

}

CLASS_REGISTER(AdcDmaDataSource, "1.0")
