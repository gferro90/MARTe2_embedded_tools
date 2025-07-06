/**
 * @file GpioReader.h
 * @brief Header file for class GpioReader
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

 * @details This header file contains the declaration of the class GpioReader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GPIO_GPIOREADER_H_
#define GPIO_GPIOREADER_H_

#include "MemoryDataSourceI.h"
#include QUOTE(_HAL_H)


using namespace MARTe;

/**
 * @brief DataSource to read digital inputs
 *
 * @details This data source only accepts one uint32 signal which represent the GPIO mask [set-15:0]
 */
class GpioReader : public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    GpioReader();

    /**
     * @brief Destructor
     */
    virtual ~GpioReader();

    /**
     * @brief Configures the data source
     * @details User must define the following parameters:
     *   - Identifier : the GPIO identifier (GPIOA, GPIOB ...)
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Read the GPIO registers to the uint32 signal
     */
    bool Synchronise();

    /**
     * @brief Checks that only one single uint32 signal is defined
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief Returns MemoryMapSynchronisedInputBroker
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief Returns true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Returns the GPIO handle
     */
    GPIO_TypeDef * GetHwHandle();


private:

    /**
     * The GPIO handle
     */
    GPIO_TypeDef *gpioHandlePtr ;

};

#endif /* GPIOREADER_H_ */
