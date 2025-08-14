/**
 * @file AdcDmaDataSource.h
 * @brief Header file for class AdcDmaDataSource
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

 * @details This header file contains the declaration of the class ADCDMA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCES_AdcDmaDataSource_ADCDMA_H_
#define DATASOURCES_AdcDmaDataSource_ADCDMA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryDataSourceI.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief ADC acquisition using DMA
 *
 * @details The user must configure a number of uint16 signals equal to the number of channels enabled for conversion in STM32CubeMX. He must configure
 * also the ADC handler and the timer triggering the channel set sampling (also set in STM32CubeMX). The user can also configure the multi-channel sampling
 * frequency and the number of buffers. The DMA autonomously writes on the circular buffer and the data source reads following the DMA and writing
 * the values transferred from the DMA to the GAM. If for some reason the data source is going to read the buffer being written by the DMA, then it waits one cycle
 * providing to the GAM again the value of the old buffer.
 *
 * @details Follows an example of configuration
 * <pre>
 * +ADC = {
 *     Class = AdcDmaDataSource
 *     Identifier = hadc1 //The ADC handle
 *     TriggerTimer = htim5 //The timer triggering the ADC multi-channel sampling
 *     ConversionFrequency = 100 //The multi-channel sampling frequency
 *     NumberOfBuffers = 5 //Number of buffer (circular). It's recommended to set this number >2.
 *     Signals = {
 *         Channel0 = {
 *             Type = uint16
 *         }
 *         ...
 *     }
 * }
 * </pre>
 */
class AdcDmaDataSource: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    AdcDmaDataSource();

    /**
     * @brief Destructor. It stops the DMA
     */
    virtual ~AdcDmaDataSource();

    /**
     * @brief Initialises the component
     * @details The user can set the following parameters:
     *   - Identifier: the ADC handle identifier
     *   - TriggerTimer: the trigger timer identifier
     *   - ConversionFrequency: the timer frequency
     *   - NumberOfBuffer: the internal circular buffer number of buffers
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Returns the offset of the memory to be copied to the GAM.
     * @details The offset depends on the incremental index for the buffer to be written to the GAM
     */
    virtual bool GetInputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset);


    /**
     * @brief Returns the broker name (MemoryMapSynchronisedMultiBufferInputBroker)
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief Checks that all the signals are uint16 and scalar
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);


    /**
     * @brief Starts the trigger timer and the DMA
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Increments the buffer index if the DMA is at least one step forward.
     */
    bool Synchronise();

private:
    /**
     * The ADC Handle
     */
    ADC_HandleTypeDef *adcDmaDataSourceHandlePtr;

    /**
     * The trigger timer handle
     */
    TIM_HandleTypeDef *triggerTimerHandlePtr;

    /**
     * The number of ADC samples per channel
     */
    uint16 nSignalElementsLocal;

    /**
     * Is started flag
     */
    bool started;

    /**
     * Last buffer written to GAM
     */
    uint8 writtenIdx;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* AdcDmaDataSource_ADCDMA_H_ */

