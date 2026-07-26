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
    numberOfDirSignals = 0u;
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
        if (!data.Read("NumDirSignals", numberOfDirSignals)) {
            numberOfDirSignals = 1u;
            REPORT_ERROR(ErrorManagement::Warning, "NumDirSignals not defined. Set to %d", numberOfDirSignals);
        }
    }

    return ret;
}

bool MotorControlGAM::Setup() {
    bool ret = true;
    if (ret) {
        GetSignalNumberOfElements(OutputSignals, numberOfDirSignals + 1u, numberOfMotors);

        iError = new float32[numberOfMotors];
        error_1 = new int32[numberOfMotors];
        directionPin = new uint8[numberOfMotors];
        endSwitchPin = new uint8[2 * numberOfMotors];
        pwmMin = new uint32[numberOfMotors];
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
        uint8 readPins = 0u;
        for (uint8 i = 0u; (i < numberOfDirSignals) && ret; i++) {
            TypeDescriptor td = GetSignalType(OutputSignals, i + 1u);
            ret = (td == UnsignedInteger32Bit);
            if (ret) {
                ret = signalsDatabase.MoveAbsolute("OutputSignals");
                if (ret) {
                    ret = signalsDatabase.MoveToChild(i + 1u);
                    if (ret) {
                        AnyType at = signalsDatabase.GetType("DirectionPins");
                        ret = !at.IsVoid();
                        if (ret) {
                            uint32 nElementsPins = at.GetNumberOfElements(0u);
                            Vector<uint8> directioPinVec(&directionPin[readPins], nElementsPins);
                            ret = signalsDatabase.Read("DirectionPins", directioPinVec);
                            if (ret) {
                                for (uint8 j = 0u; j < nElementsPins; j++) {
                                    directionPin[j + readPins] += (i * 32u);
                                }
                                readPins += nElementsPins;
                            } else {
                                REPORT_ERROR(ErrorManagement::FatalError, "Unable to read DirectionPins");
                            }
                        } else {
                            REPORT_ERROR(ErrorManagement::FatalError, "Unable to read DirectionPins");
                        }
                    } else {
                        REPORT_ERROR(ErrorManagement::FatalError, "Unable to move to direction pin signal %d", i + 1u);
                    }
                }
            } else {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal %d must be uint32", i + 1u);
            }
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
            uint32 outputsIdx = 1u + numberOfDirSignals;
            TypeDescriptor td = GetSignalType(OutputSignals, outputsIdx);
            ret = (td == Float32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal %d must be float32", outputsIdx);
            }
        }
        if (ret) {
            uint32 positionsIdx = 2u + numberOfDirSignals;
            TypeDescriptor td = GetSignalType(OutputSignals, positionsIdx);
            ret = (td == SignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "The type of output signal %d must be int32", positionsIdx);
            }
        }
        if (ret) {
            uint32 numberOfMeasures = 0u;
            GetSignalNumberOfElements(OutputSignals, 2u + numberOfDirSignals, numberOfMeasures);

            ret = (numberOfMotors == numberOfMeasures);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "The number of elements of output signal 3 must be equal to the number of motors");
            }
        }
        signalsDatabase.MoveAbsolute("OutputSignals");
        uint32 pwmStartIndex = (3u + numberOfDirSignals);
        for (uint32 i = pwmStartIndex; (i < numberOfOutputSignals) && ret; i++) {
            uint32 pwmIdx=(i-pwmStartIndex);
            if (signalsDatabase.MoveToChild(i)) {
                signalsDatabase.Read("PwmMin", pwmMin[pwmIdx]);
                ret = signalsDatabase.Read("Kp", kp[pwmIdx]);
                if (ret) {
                    if (!signalsDatabase.Read("Ki", ki[pwmIdx])) {
                        ki[pwmIdx] = 0.;
                    }
                    if (!signalsDatabase.Read("Kd", kd[pwmIdx])) {
                        kd[pwmIdx] = 0.;
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
        output = (float32*) (outputSignalsMemoryIndexer[numberOfDirSignals + 1]);
        position = (int32*) (outputSignalsMemoryIndexer[numberOfDirSignals + 2]);
        pwm = (uint32*) (outputSignalsMemoryIndexer[numberOfDirSignals + 3]);
    }

    return ret;
}

bool MotorControlGAM::Execute() {

    float32 dt = 1.;
    if (timestamp_1 > 0ull) {
        dt = (float32)((*timestampUs - timestamp_1) / 1000000.0);
    }
    for (uint8 i = 0u; i < numberOfDirSignals; i++) {
        directionMask[i] = 0u;
    }
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

        //REPORT_ERROR(ErrorManagement::Information, "%d: reference=%d, measure=%d", i, reference[i], measure[i]);

        int32 error = (reference[i] - measure[i]);
        float32 curOutput = output[i];

        output[i] = (kp[i] * (float32)(error));
        //REPORT_ERROR(ErrorManagement::Information, "%d: Step1 output=%f, kp=%f, ki=%f, kd=%f", i, output[i], kp[i], ki[i], kd[i]);

        if (timestamp_1 > 0ull) {
            float32 dError = ((float32)(error - error_1[i])) / dt;
            //accumulate if not moving
            if ((error == error_1[i]) && (!isSwitch)) {
                iError[i] += (error_1[i] * dt);
            }
            output[i] += (ki[i] * iError[i]) + (kd[i] * dError);
        }
        //REPORT_ERROR(ErrorManagement::Information, "%d: Step2 output=%f", i, output[i]);

        if (output[i] > outMax) {
            output[i] = outMax;
        } else if (output[i] < (-outMax)) {
            output[i] = -outMax;
        }

        if ((error < deadBand) && (error > (-deadBand))) {
            output[i] = 0.;
            iError[i] = 0;
        }
        //REPORT_ERROR(ErrorManagement::Information, "%d: Step3 output=%f", i, output[i]);

        //if switch and keep same direction, hold on
        if (isSwitch1 && (error > 0)) {
            output[i] = 0u;
        }
        if (isSwitch2 && (error < 0)) {
            output[i] = 0u;
        }

        //REPORT_ERROR(ErrorManagement::Information, "%d: Step4 output=%f", i, output[i]);

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
            uint8 dirIdx = (directionPin[i] / 32);
            uint8 dirPin = (directionPin[i] % 32);
            directionMask[dirIdx] |= (1 << dirPin);
        } else {
            //set voltage to both motor terminals
            pwm[i] = pwmMax;
            uint8 dirIdx = (directionPin[i] / 32);
            uint8 dirPin = (directionPin[i] % 32);
            directionMask[dirIdx] |= (1 << dirPin);
        }
        //REPORT_ERROR(ErrorManagement::Information, "%d: Step5 output=%f", i, output[i]);

        error_1[i] = error;
        //REPORT_ERROR(ErrorManagement::Information, "%d: error=%d, pwm=%d, output=%f", i, error, pwm[i], output[i]);
        //REPORT_ERROR(ErrorManagement::Information, "%d: pwmMin=%d, directionMask=%d", i, pwmMin[i], *directionMask);

    }
    timestamp_1 = *timestampUs;
    return true;
}

CLASS_REGISTER(MotorControlGAM, "1.0")
