/**
 * @file GpioDataSource.h
 * @brief Header file for class GpioDataSource
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

 * @details This header file contains the declaration of the class GpioDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GPIO_GPIODATASOURCE_H_
#define GPIO_GPIODATASOURCE_H_

#include "MemoryDataSourceI.h"
#include QUOTE(_HAL_H)


using namespace MARTe;

/**
 * @brief DataSource to read and write digital IOs
 *
 * @details This data source only accepts one uint32 signal which represent the GPIO mask [reset-31:16][set-15:0]
 */
class GpioDataSource : public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    GpioDataSource();

    /**
     * @brief Destructor
     */
    virtual ~GpioDataSource();

    /**
     * @brief Configures the data source
     * @details The user must define
     *  - Identifier : the GPIO identifier (GPIOA, GPIOB, ecc)
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Resets current output and then set the value of the signal.
     */
    virtual bool Synchronise();

    /**
     * @brief Checks that only one uint32 signal is declared
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief Return MemoryMapSynchronisedOutputBroker
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief return true
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);



private:

    /**
     * The GPIO handle
     */
    GPIO_TypeDef *gpioHandlePtr[10];

    uint16 inputMask[10];

    uint16 outputMask[10];

    uint16 setMask[10];

    uint16 resetMask[10];

    uint32 numberOfInputs;

};

#endif /* GPIODATASOURCE_H_ */
