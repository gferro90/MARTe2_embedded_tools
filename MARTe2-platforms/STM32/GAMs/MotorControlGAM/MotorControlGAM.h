/**
 * @file MotorControlGAM.h
 * @brief Header file for class BufferGAM
 * @date 6 Aug 2016
 * @author andre
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

 * @details This header file contains the declaration of the class BufferGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SINEGENERATOR_H_
#define SINEGENERATOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAM.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 */
class MotorControlGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    MotorControlGAM();

    /**
     * @brief Destructor
     */
    virtual ~MotorControlGAM();

    /**
     * @brief Initialises the GAM.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Generates the sine waveforms.
     */
    virtual bool Execute();

    /**
     * @brief Checks signal types and dimensions.
     */
    virtual bool Setup();


private:

    uint32 numberOfMotors;

    uint8 *directionPin;

    uint8 *endSwitchPin;

    uint32 *endSwitchOut;

    uint32 *endSwitches;

    int32 *reference;

    int32 *measure;

    float32 *output;

    uint32 *pwm;

    uint32 *directionMask;

    uint64 *timestampUs;

    uint64 timestamp_1;

    float32 *iError;

    int32 *error_1;

    float32 kp;

    float32 ki;

    float32 kd;

    float32 outMax;

    uint32 *pwmMin;

    uint32 pwmMax;

    int32 deadBand;

    int8 *errorAtSwitch;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SINEGENERATOR_H_ */

