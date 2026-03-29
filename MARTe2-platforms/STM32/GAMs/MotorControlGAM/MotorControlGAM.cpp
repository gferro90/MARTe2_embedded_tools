/**
 * @file MotorControlGAM.cpp
 * @brief Source file for class BufferGAM
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

 * @details This source file contains the definition of all the methods for
 * the class BufferGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MotorControlGAM.h"
#include "Vector.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
MotorControlGAM::MotorControlGAM() :
        GAM() {
    numberOfMotors = 0u;
    directionPin = NULL;
    endSwitchPin = NULL;

    measure = NULL;
    reference = NULL;
    endSwitches = NULL;

    pwm = NULL;
    output = NULL;
    directionMask = NULL;
    endSwitchOut = NULL;
    position = NULL;

    error_1 = 0;
    iError = 0;

    timestampUs = NULL;
    timestamp_1 = 0ull;

    error_1 = NULL;
    iError = NULL;

    kp = NULL;
    ki = NULL;
    kd = NULL;

    outMax = 24000.;
    pwmMin = NULL;
    pwmMax = 1000u;

    deadBand = 0;
    errorAtSwitch = NULL;
}

MotorControlGAM::~MotorControlGAM() {
    if (directionPin != NULL) {
        delete[] directionPin;
    }
    if (endSwitchPin != NULL) {
        delete[] endSwitchPin;
    }
    if (pwmMin != NULL) {
        delete[] pwmMin;
    }
    if (error_1 != NULL) {
        delete[] error_1;
    }
    if (iError != NULL) {
        delete[] iError;
    }
    if (errorAtSwitch != NULL) {
        delete[] iError;
    }
    if (kp != NULL) {
        delete[] kp;
    }
    if (ki != NULL) {
        delete[] ki;
    }
    if (kd != NULL) {
        delete[] kd;
    }
}

bool MotorControlGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        if (!data.Read("MaxOutput", outMax)) {
            outMax = 24000.;
            REPORT_ERROR(ErrorManagement::Warning, "MaxOutput not defined. Set to %d", outMax);
        }
        if (!data.Read("DeadBand", deadBand)) {
            deadBand = 0;
            REPORT_ERROR(ErrorManagement::Warning, "DeadBand not defined. Set to %d", deadBand);
        }
        if (!data.Read("MaxPwm", pwmMax)) {
            pwmMax = 1000u;
            REPORT_ERROR(ErrorManagement::Warning, "MaxPwm not defined. Set to %d", pwmMax);
        }
    }

    return ret;
}

bool MotorControlGAM::Setup() {
    bool ret = true;
    if (ret) {
        GetSignalNumberOfElements(OutputSignals, 2u, numberOfMotors);

        iError = new float32[numberOfMotors];
        error_1 = new int32[numberOfMotors];
        directionPin = new uint8[numberOfMotors];
        endSwitchPin = new uint8[2 * numberOfMotors];
        pwmMin = new uint32[numberOfMotors];
        errorAtSwitch = new int8[numberOfMotors];
        kp = new float32[numberOfMotors];
        ki = new float32[numberOfMotors];
        kd = new float32[numberOfMotors];

    }
    if (ret) {
        TypeDescriptor td = GetSignalType(InputSignals, 0u);
        ret = (td == UnsignedInteger64Bit);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The type of input signal 0 must be uint64");
        }
        if (ret) {
            TypeDescriptor td = GetSignalType(InputSignals, 1u);
            ret = (td == UnsignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of input signal 1 must be uint32");
            }
        }
        for (uint32 i = 2u; (i < numberOfInputSignals) && ret; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            ret = (td == SignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of input signal %d must be int32", i);
            }
        }
        for (uint32 i = 0u; (i < numberOfMotors) && ret; i++) {
            iError[i] = 0.;
            error_1[i] = 0;
            errorAtSwitch = 0;
        }
    }
    if (ret) {
        timestampUs = (uint64*) (inputSignalsMemoryIndexer[0]);
        endSwitches = (uint32*) (inputSignalsMemoryIndexer[1]);
        reference = (int32*) (inputSignalsMemoryIndexer[2]);
        measure = (int32*) (inputSignalsMemoryIndexer[3]);
    }
    if (ret) {
        ret = (numberOfOutputSignals >= 5);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The number of output signals shall be equal or greater than 5");
        }
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(OutputSignals, 0u);
        ret = (td == UnsignedInteger32Bit);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal 0 must be uint32");
        }
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(OutputSignals, 1u);
        ret = (td == UnsignedInteger32Bit);
        if (ret) {
            ret = signalsDatabase.MoveAbsolute("OutputSignals");
            if (ret) {
                Vector<uint8> directioPinVec(directionPin, numberOfMotors);
                signalsDatabase.MoveToChild(1u);
                ret = signalsDatabase.Read("DirectionPins", directioPinVec);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Unable to read DirectionPins");
                }
            }
        } else {
            REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal 1 must be uint32");
        }
        if (ret) {
            ret = signalsDatabase.MoveAbsolute("InputSignals");
            if (ret) {
                Vector<uint8> endSwitchPinVec(endSwitchPin, 2 * numberOfMotors);
                signalsDatabase.MoveToChild(1u);
                ret = signalsDatabase.Read("EndSwitchPins", endSwitchPinVec);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::FatalError, "Unable to read EndSwitchPins");
                }
            }
        }
        if (ret) {
            TypeDescriptor td = GetSignalType(OutputSignals, 2u);
            ret = (td == Float32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal 2 must be float32");
            }
        }
        if (ret) {
            TypeDescriptor td = GetSignalType(OutputSignals, 3u);
            ret = (td == SignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal 3 must be int32");
            }
        }
        if (ret) {
            uint32 numberOfMeasures = 0u;
            GetSignalNumberOfElements(OutputSignals, 3u, numberOfMeasures);

            ret = (numberOfMotors == numberOfMeasures);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of elements of output signal 3 must be equal to the number of motors");
            }
        }
        signalsDatabase.MoveAbsolute("OutputSignals");
        for (uint32 i = 4u; (i < numberOfOutputSignals) && ret; i++) {
            if (signalsDatabase.MoveToChild(i)) {
                signalsDatabase.Read("PwmMin", pwmMin[i - 4u]);
                ret = signalsDatabase.Read("Kp", kp[i - 4u]);
                if (ret) {
                    if (!signalsDatabase.Read("Ki", ki[i - 4u])) {
                        ki[i - 4u] = 0.;
                    }
                    if (!signalsDatabase.Read("Kd", kd[i - 4u])) {
                        kd[i - 4u] = 0.;
                    }
                } else {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Please define Kp in Pwm signal %d", i);
                }
                signalsDatabase.MoveToAncestor(1u);
            }
            TypeDescriptor td = GetSignalType(OutputSignals, i);
            ret = (td == UnsignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal %d must be uint32", i);
            }
        }
    }
    if (ret) {
        endSwitchOut = (uint32*) (outputSignalsMemoryIndexer[0]);
        directionMask = (uint32*) (outputSignalsMemoryIndexer[1]);
        output = (float32*) (outputSignalsMemoryIndexer[2]);
        position = (int32*) (outputSignalsMemoryIndexer[3]);
        pwm = (uint32*) (outputSignalsMemoryIndexer[4]);
    }

    return ret;
}

bool MotorControlGAM::Execute() {

    float32 dt = 1.;
    if (timestamp_1 > 0ull) {
        dt = (float32)((*timestampUs - timestamp_1) / 1000000.0);
    }
    *directionMask = 0u;
    *endSwitchOut = 0u;

    for (uint32 i = 0; i < numberOfMotors; i++) {

        bool isSwitch1 = (((*endSwitches) & (1 << endSwitchPin[2 * i])) > 0);
        bool isSwitch2 = (((*endSwitches) & (1 << endSwitchPin[(2 * i) + 1])) > 0);

        if (i < 16) {
            *endSwitchOut |= (((uint8) isSwitch1) << (2 * i));
            *endSwitchOut |= (((uint8) isSwitch2) << (2 * i + 1));
        }
        bool isSwitch = (isSwitch1 || isSwitch2);
        position[i] = measure[i];

        int32 error = (reference[i] - measure[i]);
        float32 curOutput = output[i];

        output[i] = (kp[i] * (float32)(error));

        if (timestamp_1 > 0ull) {
            float32 dError = ((float32)(error - error_1[i])) / dt;
            //accumulate if not moving
            if (error == error_1[i]) {
                iError[i] += (error_1[i] * dt);
            }
            output[i] += (ki[i] * iError[i]) + (kd[i] * dError);
        }

        if (output[i] > outMax) {
            output[i] = outMax;
        } else if (output[i] < (-outMax)) {
            output[i] = -outMax;
        }

        if ((error < deadBand) && (error > (-deadBand))) {
            output[i] = 0.;
            iError[i] = 0;
        }

        //if switch and keep same direction, hold on
        if (isSwitch && (error != 0)) {
            if (errorAtSwitch[i] == 0) {
                //save error at switch
                errorAtSwitch[i] = (error > 0) ? (1) : (-1);
            }
            if ((errorAtSwitch[i] * error) > 0) {
                output[i] = 0u;
            }
        } else {
            errorAtSwitch[i] = 0;
        }

        if (output[i] > 0) {
            pwm[i] = (uint32)((output[i] * pwmMax) / outMax);
            if (pwm[i] < pwmMin[i]) {
                pwm[i] = pwmMin[i];
            }
            //reset gpio
        } else if (output[i] < 0) {
            pwm[i] = pwmMax - (uint32)(((-output[i]) * pwmMax) / outMax);
            if (pwm[i] > (pwmMax - pwmMin[i])) {
                pwm[i] = (pwmMax - pwmMin[i]);
            }
            //set gpio
            *directionMask |= (1 << (directionPin[i]));
        } else {
            //set voltage to both motor terminals
            pwm[i] = pwmMax;
            *directionMask |= (1 << (directionPin[i]));
        }

        error_1[i] = error;
        //REPORT_ERROR(ErrorManagement::Information, "%d: error=%d, pwm=%d, output=%f", i, error, pwm[i], output[i]);
        //REPORT_ERROR(ErrorManagement::Information, "%d: pwmMin=%d, directionMask=%d", i, pwmMin[i], *directionMask);

    }
    timestamp_1 = *timestampUs;
    return true;
}

CLASS_REGISTER(MotorControlGAM, "1.0")
