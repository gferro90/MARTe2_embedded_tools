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

    measure = NULL;
    reference = NULL;

    pwm = NULL;
    output = NULL;
    directionMask = NULL;
    error_1 = 0;
    iError = 0;

    timestampUs = NULL;
    timestamp_1 = 0ull;

    error_1 = NULL;
    iError = NULL;

    kp = 0.;
    ki = 0.;
    kd = 0.;

    outMax = 24000.;
    pwmMin = NULL;
    pwmMax = 1000u;

    deadBand = 0;
}

MotorControlGAM::~MotorControlGAM() {
    if (directionPin != NULL) {
        delete[] directionPin;
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
}

bool MotorControlGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        ret = data.Read("Kp", kp);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::InitialisationError", "Please define PID Kp");
        }
    }
    if (ret) {
        ret = data.Read("Ki", ki);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::InitialisationError", "Please define PID Ki");
        }
    }
    if (ret) {
        ret = data.Read("Kd", kd);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::InitialisationError", "Please define PID Kd");
        }
    }
    if (ret) {
        if (!data.Read("MaxOutput", outMax)) {
            outMax = 24000.;
            REPORT_ERROR("ErrorManagement::InitialisationError", "MaxOutput not defined. Set to %d", outMax);
        }
        if (!data.Read("DeadBand", deadBand)) {
            deadBand = 0;
            REPORT_ERROR("ErrorManagement::InitialisationError", "DeadBand not defined. Set to %d", deadBand);
        }
        if (!data.Read("MaxPwm", pwmMax)) {
            pwmMax = 1000u;
            REPORT_ERROR("ErrorManagement::InitialisationError", "MaxPwm not defined. Set to %d", pwmMax);
        }
    }

    return ret;
}

bool MotorControlGAM::Setup() {
    bool ret = (numberOfInputSignals == 3u);
    if (!ret) {
        REPORT_ERROR("ErrorManagement::InitialisationError", "The number of input signals shall be equal to 3");
    }
    if (ret) {
        GetSignalNumberOfElements(InputSignals, 1u, numberOfMotors);

        iError = new float32[numberOfMotors];
        error_1 = new int32[numberOfMotors];
        directionPin = new uint8[numberOfMotors];
        pwmMin = new uint32[numberOfMotors];

        uint32 numberOfMeasures = 0u;
        GetSignalNumberOfElements(InputSignals, 1u, numberOfMeasures);
        ret = (numberOfMotors == numberOfMeasures);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::InitialisationError", "The number of elements of input signals 2 and 3 must be equal");
        }
    }
    if (ret) {
        ret = (numberOfOutputSignals >= 3);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::InitialisationError", "The number of output signals shall be equal or greater than 3");
        }
    }
    if (ret) {
        ret = ((numberOfOutputSignals - 2u) == numberOfMotors);
        REPORT_ERROR("ErrorManagement::InitialisationError", "The number of output signals shall be equal to number of motors +2");
    }
    if (ret) {
        uint32 numberOfControls = 0u;
        GetSignalNumberOfElements(OutputSignals, 1u, numberOfControls);

        ret = (numberOfMotors == numberOfControls);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::InitialisationError", "The number of elements input and output signal 1 must be equal");
        }
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(InputSignals, 0u);
        ret = (td == UnsignedInteger64Bit);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::FatalError", "The type of input signal 0 must be uint64");
        }
        for (uint32 i = 1u; (i < numberOfInputSignals) && ret; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            ret = (td == SignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR("ErrorManagement::FatalError", "The type of input signal %d must be int32", i);
            }
            if (i < numberOfMotors) {
                iError[i] = 0.;
                error_1[i] = 0;
            }
        }
    }
    if (ret) {
        timestampUs = (uint64*) (inputSignalsMemoryIndexer[0]);
        reference = (int32*) (inputSignalsMemoryIndexer[1]);
        measure = (int32*) (inputSignalsMemoryIndexer[2]);
    }
    if (ret) {
        TypeDescriptor td = GetSignalType(OutputSignals, 0u);
        ret = (td == UnsignedInteger32Bit);
        if (ret) {
            ret = signalsDatabase.MoveAbsolute("OutputSignals");
            if (ret) {
                Vector<uint8> directioPinVec(directionPin, numberOfMotors);
                signalsDatabase.MoveToChild(0u);
                signalsDatabase.Read("DirectionPins", directioPinVec);
            }
        } else {
            REPORT_ERROR("ErrorManagement::FatalError", "The type of output signal 0 must be uint32");
        }
        if (ret) {
            TypeDescriptor td = GetSignalType(OutputSignals, 1u);
            ret = (td == Float32Bit);
            if (!ret) {
                REPORT_ERROR("ErrorManagement::FatalError", "The type of output signal 1 must be float32");
            }
        }
        signalsDatabase.MoveAbsolute("OutputSignals");
        for (uint32 i = 2u; (i < numberOfOutputSignals) && ret; i++) {
            if (signalsDatabase.MoveToChild(i)) {
                signalsDatabase.Read("PwmMin", pwmMin[i - 2u]);
                signalsDatabase.MoveToAncestor(1u);
            }
            TypeDescriptor td = GetSignalType(OutputSignals, i);
            ret = (td == UnsignedInteger32Bit);
            if (!ret) {
                REPORT_ERROR("ErrorManagement::FatalError", "The type of output signal %d must be uint32", i);
            }
        }
    }
    if (ret) {
        directionMask = (uint32*) (outputSignalsMemoryIndexer[0]);
        output = (float32*) (outputSignalsMemoryIndexer[1]);
        pwm = (uint32*) (outputSignalsMemoryIndexer[2]);
    }

    return ret;
}

bool MotorControlGAM::Execute() {
    float32 dt = 1.;
    if (timestamp_1 > 0ull) {
        dt = (float32)((*timestampUs - timestamp_1) / 1000000.0);
    }
    *directionMask = 0u;
    for (uint32 i = 0; i < numberOfMotors; i++) {
        int32 error = (reference[i] - measure[i]);

        output[i] = (kp * (float32)(error));

        if (timestamp_1 > 0ull) {
            float32 dError = ((float32)(error - error_1[i])) / dt;
            iError[i] += (error_1[i] * dt);
            output[i] += (ki * iError[i]) + (kd * dError);
        }

        if (output[i] > outMax) {
            output[i] = outMax;
        } else if (output[i] < (-outMax)) {
            output[i] = -outMax;
        }

        if ((error < deadBand) && (error > (-deadBand))) {
            output[i] = 0u;
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
            pwm[i] = 0u;
            //reset gpio
        }

        error_1[i] = error;
        //REPORT_ERROR(ErrorManagement::Information, "%d: error=%d, pwm=%d, output=%f", i, error, pwm[i], output[i]);
        //REPORT_ERROR(ErrorManagement::Information, "%d: pwmMin=%d, directionMask=%d", i, pwmMin[i], *directionMask);

    }
    timestamp_1 = *timestampUs;
    return true;
}

CLASS_REGISTER(MotorControlGAM, "1.0")
