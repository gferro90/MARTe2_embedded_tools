/**
 * @file DacDmaDataSource.h
 * @brief Header file for class DacDmaDataSource
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
 * @brief DAC generation using DMA
 *
 * @details The user must configure a number of uint32 signals equal to the number of channels enabled for conversion in STM32CubeMX. He must configure
 * also the DAC handler and the timer triggering the channel sampling out (also set in STM32CubeMX). The user can also configure the channel sampling out
 * frequency and the number of buffers. The data source (broker) writes the data from the GAM to the internal circular buffer and the DMA autonomously transfers
 * the samples to the pheripheral reading from the circular buffer. If for some reason the DMA is going to read the buffer being written by the data source, then
 * it waits one cycle transferring the old buffer to the pheripheral
 *
 * @details Follows an example of configuration
 * <pre>
 * +DAC = {
 *     Class = DacDmaDataSource
 *     Identifier = hdac //The DAC handle
 *     TriggerTimer = htim7 //The timer triggering the DAC sampling out
 *     ConversionFrequency = 100 //The sampling out frequency
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
class DacDmaDataSource: public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    DacDmaDataSource();

    /**
     * @brief Destructor. It stops the DMA
     */
    virtual ~DacDmaDataSource();

    /**
     * @brief Initialises the component
     * @details The user can set the following parameters:
     *   - Identifier: the DAC handle identifier
     *   - TriggerTimer: the trigger timer identifier
     *   - ConversionFrequency: the timer frequency
     *   - NumberOfBuffer: the internal circular buffer number of buffers
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Returns the broker name (MemoryMapSynchronisedMultiBufferOutputBroker)
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
     * @brief Returns the offset of the memory to be copied to the GAM.
     * @details The offset depends on the incremental index for the buffer to be written to the GAM
     */
    virtual bool GetOutputOffset(const uint32 signalIdx, const uint32 numberOfSamples, uint32 &offset);

    /**
     * @brief Returns true
     */
    bool Synchronise();

private:
    /**
     * The ADC Handle
     */
    DAC_HandleTypeDef *dacDmaDataSourceHandlePtr;

    /**
     * The trigger timer handle
     */
    TIM_HandleTypeDef *triggerTimerHandlePtr;
    /**
     * Is started flag
     */
    bool started;

    /**
     * The number of elements for each channels
     */
    uint16 nSignalElementsLocal[2];
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* AdcDmaDataSource_ADCDMA_H_ */

