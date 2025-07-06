/**
 * @file DacDmaDataSource.cpp
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
 * the class DacDmaDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "DacDmaDataSource.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static uint8 _dac_dma_writtenIdxCh[2] = { 0u, 0u };
static uint8 _dac_dma_readIdx = 0u;
static uint8 _dac_dma_numberOfBuffers = 0u;
static uint32 _dac_dma_sizeCh[2] = { 0u, 0u };
static uint16 *_dac_dma_dacBufferCh[2] = { NULL, NULL };
static uint16 *_dac_dma_dsBufferCh[2] = { NULL, NULL };

void HAL_DAC_ConvHalfCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
    uint8 newReadBuffer = (_dac_dma_readIdx + 1u) % _dac_dma_numberOfBuffers;
    if ((_dac_dma_writtenIdxCh[0] != _dac_dma_readIdx) && (_dac_dma_writtenIdxCh[0] != newReadBuffer)) {
        MemoryOperationsHelper::Copy(&(_dac_dma_dacBufferCh[0][0]), &(_dac_dma_dsBufferCh[0][_dac_dma_writtenIdxCh[0]*_dac_dma_sizeCh[0]]), sizeof(uint16) * _dac_dma_sizeCh[0]);
        _dac_dma_writtenIdxCh[0]++;
        _dac_dma_writtenIdxCh[0] %= _dac_dma_numberOfBuffers;
    }
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef *hdac) {
    uint8 newReadBuffer = (_dac_dma_readIdx + 1u) % _dac_dma_numberOfBuffers;
    if ((_dac_dma_writtenIdxCh[0] != _dac_dma_readIdx) && (_dac_dma_writtenIdxCh[0] != newReadBuffer)) {
        MemoryOperationsHelper::Copy(&(_dac_dma_dacBufferCh[0][_dac_dma_sizeCh[0]]), &(_dac_dma_dsBufferCh[0][_dac_dma_writtenIdxCh[0]*_dac_dma_sizeCh[0]]), sizeof(uint16) * _dac_dma_sizeCh[0]);
        _dac_dma_writtenIdxCh[0]++;
        _dac_dma_writtenIdxCh[0] %= _dac_dma_numberOfBuffers;
    }
}

void HAL_DACEx_ConvHalfCpltCallbackCh2(DAC_HandleTypeDef *hdac) {
    uint8 newReadBuffer = (_dac_dma_readIdx + 1u) % _dac_dma_numberOfBuffers;
    if ((_dac_dma_writtenIdxCh[1] != _dac_dma_readIdx) && (_dac_dma_writtenIdxCh[1] != newReadBuffer)) {
        MemoryOperationsHelper::Copy(&(_dac_dma_dacBufferCh[1][0]), &(_dac_dma_dsBufferCh[1][_dac_dma_writtenIdxCh[1]*_dac_dma_sizeCh[1]]), sizeof(uint16) * _dac_dma_sizeCh[0]);
        _dac_dma_writtenIdxCh[1]++;
        _dac_dma_writtenIdxCh[1] %= _dac_dma_numberOfBuffers;
    }
}

void HAL_DACEx_ConvCpltCallbackCh2(DAC_HandleTypeDef *hdac) {
    uint8 newReadBuffer = (_dac_dma_readIdx + 1u) % _dac_dma_numberOfBuffers;
    if ((_dac_dma_writtenIdxCh[1] != _dac_dma_readIdx) && (_dac_dma_writtenIdxCh[1] != newReadBuffer)) {
        MemoryOperationsHelper::Copy(&(_dac_dma_dacBufferCh[1][_dac_dma_sizeCh[1]]), &(_dac_dma_dsBufferCh[1][_dac_dma_writtenIdxCh[1]*_dac_dma_sizeCh[1]]), sizeof(uint16) * _dac_dma_sizeCh[0]);
        _dac_dma_writtenIdxCh[1]++;
        _dac_dma_writtenIdxCh[1] %= _dac_dma_numberOfBuffers;
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DacDmaDataSource::DacDmaDataSource() :
        MemoryDataSourceI() {
//Initialisation already done by the tool !!!
    triggerTimerHandlePtr = NULL;
    started = false;
    nSignalElementsLocal[0] = 0u;
    nSignalElementsLocal[1] = 0u;
}

DacDmaDataSource::~DacDmaDataSource() {
    if (started) {
        uint32 channels[] = { DAC_CHANNEL_1, DAC_CHANNEL_2 };

        for (uint32 i = 0u; (i < numberOfSignals); i++) {
            HAL_DAC_Stop_DMA(dacDmaDataSourceHandlePtr, channels[i]);
            if (_dac_dma_dacBufferCh[i] != NULL) {
                void *ptrToFree=(void*)(_dac_dma_dacBufferCh[i]);
                HeapManager::Free(ptrToFree);
            }
        }
    }
}

bool DacDmaDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString adcId;
        ret = data.Read("Identifier", adcId);
        if (ret) {
            dacDmaDataSourceHandlePtr = (DAC_HandleTypeDef*) GetHandle(adcId.Buffer());
            ret = (dacDmaDataSourceHandlePtr != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "DAC handler is not valid");
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "DAC not specified");
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
                REPORT_ERROR(ErrorManagement::Information, "ConversionFrequency %f approximated to %f", convFreq, realFreq);

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

bool DacDmaDataSource::GetOutputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset) {
    offset = (_dac_dma_readIdx * nSignalElementsLocal[signalIdx] * sizeof(uint16));
    return true;
}

bool DacDmaDataSource::Synchronise() {
    _dac_dma_readIdx++;
    _dac_dma_readIdx %= numberOfBuffers;
    return true;
}

bool DacDmaDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        uint32 numberOfSignals = GetNumberOfSignals();
        ret = (numberOfSignals <= 2);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The DAC only supports two channels");
        }
        for (uint32 i = 0u; i < numberOfSignals && ret; i++) {
            ret = (GetSignalType(i) == UnsignedInteger16Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The DacDmaDataSource signal type has to be uint16");
            }
        }
    }
    return ret;
}

const char8* DacDmaDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == OutputSignals) {
        return "MemoryMapSynchronisedMultiBufferOutputBroker";
    }

    return "";
}

bool DacDmaDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    bool ret = true;
    uint32 channels[] = { DAC_CHANNEL_1, DAC_CHANNEL_2 };
    if (!started) {
        //timer triggers DAC conversion
        ret = (HAL_TIM_Base_Start_IT(triggerTimerHandlePtr) == HAL_OK);
        _dac_dma_numberOfBuffers = (uint8)numberOfBuffers;
        if (ret) {
            uint32 offset = 0u;
            for (uint32 i = 0u; (i < numberOfSignals) && ret; i++) {
                uint32 numberOfElements;
                GetSignalNumberOfElements(i, numberOfElements);
                nSignalElementsLocal[i] = numberOfElements;
                _dac_dma_sizeCh[i] = numberOfElements;
                _dac_dma_dacBufferCh[i] = (uint16*) HeapManager::Malloc(2 * _dac_dma_sizeCh[i] * sizeof(uint16));
                _dac_dma_dsBufferCh[i] = (uint16*) (&(memory[offset]));

                ret = (HAL_DAC_Start_DMA(dacDmaDataSourceHandlePtr, channels[i], (uint32_t*) _dac_dma_dacBufferCh[i], (2 * numberOfElements), DAC_ALIGN_12B_R) == HAL_OK);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Failed to start DAC DMA");
                }
                offset += (numberOfElements * numberOfBuffers * sizeof(uint16));
            }
        } else {
            REPORT_ERROR(ErrorManagement::FatalError, "Failed to start trigger timer");
        }
        started = true;
    }
    return ret;
}

CLASS_REGISTER(DacDmaDataSource, "1.0")
