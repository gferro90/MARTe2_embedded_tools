/**
 * @file EncoderDataSource.h
 * @brief Header file for class EncoderDataSource
 * @date 08/nov/2025
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

 * @details This header file contains the declaration of the class EncoderDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GPIO_GPIOWRITER_H_
#define GPIO_GPIOWRITER_H_

#include "MemoryDataSourceI.h"
#include QUOTE(_HAL_H)


using namespace MARTe;

/**
 * @brief DataSource to read from encoder
 *
 * @details This data source is used to read encoder pulses and increment the counter accordingly. The encoder has been configured to count on both edges and on both channels (x4 configuration)
 * The uint16 counter is converted to int32 signal. The first signal of this data source is to reset the counter.
 */
class EncoderDataSource : public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    EncoderDataSource();

    /**
     * @brief Destructor
     */
    virtual ~EncoderDataSource();

    /**
     * @brief Configures the data source
     * @details The user must define
     *  - Identifier : the PWM timer identifier
     *  - Invert : Optional, to invert the counting
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Resets the counter if the first signal is 1. Reads the counter.
     */
    virtual bool Synchronise();

    /**
     * @brief Checks that the first (reset) signal is uint8 and that the second (counter) signal is int32
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief Return MemoryMapSynchronisedOutputBroker if direction is output and MemoryMapInputBroker if direction is input.
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief Starts the timer and resets the encoder counter
     */
    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    /**
     * @brief Retrieves the Timer handle
     */
    TIM_HandleTypeDef * GetHwHandle();


private:

    /**
     * The timer PWM handle
     */
    TIM_HandleTypeDef *encoderHandle;

    uint16 lastCounter;

    uint8 invert;

};

#endif /* GPIOWRITER_H_ */
