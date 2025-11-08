/**
 * @file PwmDataSource.h
 * @brief Header file for class PwmDataSource
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

 * @details This header file contains the declaration of the class PwmDataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GPIO_GPIOWRITER_H_
#define GPIO_GPIOWRITER_H_

#include "MemoryDataSourceI.h"
#include QUOTE(_HAL_H)


using namespace MARTe;

/**
 * @brief DataSource to generate PWM signal
 *
 * @details This data source is used to generate a PWM signal. The period is configured in STMCube as 1kHz, the range is 0-999 (500 is 50% duty cycle),
 * the polarity is high (signal is high for the ticks specified)
 */
class PwmDataSource : public MemoryDataSourceI {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    PwmDataSource();

    /**
     * @brief Destructor
     */
    virtual ~PwmDataSource();

    /**
     * @brief Configures the data source
     * @details The user must define
     *  - Identifier : the PWM timer identifier
     *  - StartVal : Optional, is the value of PWM at starting
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Sets the duty cycle to the channel (identified by the signal)
     */
    virtual bool Synchronise();

    /**
     * @brief Checks that only uint32 signals are declared
     */
    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

    /**
     * @brief Return MemoryMapSynchronisedOutputBroker
     */
    virtual const char8 *GetBrokerName(StructuredDataI &data,
            const SignalDirection direction);

    /**
     * @brief Starts the timer and the PWM generation
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
    TIM_HandleTypeDef *pwmHandle;

    uint32 startVal;

};

#endif /* GPIOWRITER_H_ */
