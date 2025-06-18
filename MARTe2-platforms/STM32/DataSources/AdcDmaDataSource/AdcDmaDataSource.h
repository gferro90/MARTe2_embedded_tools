/**
 * @file ADCDMA.h
 * @brief Header file for class ADCDMA
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
 * @details The user must configure a number of uint32 signals equal to the number of channels enabled for conversion in STM32CubeMX
 *
 * @details Follows an example of configuration
 * <pre>
 * +ADC = {
 *     Class = AdcDmaDataSource
 *     Identifier = hadc1 //The ADC handle
 *     Signals = {
 *         Channel0 = {
 *             Type = uint32
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
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Returns the broker name
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    /**
     * @brief Checks that all the signals are uint32 and scalar
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);


    /**
     * @brief Starts the DMA
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    /**
     * @brief Returns true
     */
    bool Synchronise();

private:
    /**
     * The ADC Handle
     */
    ADC_HandleTypeDef *AdcDmaDataSourceHandlePtr;

    /**
     * Is started flag
     */
    bool started;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* AdcDmaDataSource_ADCDMA_H_ */

