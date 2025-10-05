/**
 * @file SineGeneratorGAM.cpp
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
#include "SineGeneratorGAM.h"
#include "FastMath.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
SineGeneratorGAM::SineGeneratorGAM() :
        GAM() {
    amplitude = NULL;
    phase = NULL;
    offset = NULL;
    frequency = NULL;
    timestampUs = NULL;
    sine = NULL;
    sampleTime = 0u;
    numberOfSineElements = 1u;
    outputMin = 0u;
    outputMax = 4095u;
    inputMin = 0.;
    inputMax = 3.3;
}

SineGeneratorGAM::~SineGeneratorGAM() {

}

bool SineGeneratorGAM::Initialise(StructuredDataI &data) {
    bool ret = GAM::Initialise(data);
    if (ret) {
        if (!data.Read("SampleTimeUs", sampleTime)) {
            sampleTime = 0u;
        }
        if (!data.Read("InputMin", inputMin)) {
            inputMin = -1;
        }
        if (!data.Read("InputMax", inputMax)) {
            inputMax = 1;
        }
        if (!data.Read("OutputMin", outputMin)) {
            outputMin = 0u;
        }
        if (!data.Read("OutputMax", outputMax)) {
            outputMax = 4095u;
        }
    }
    return ret;
}

bool SineGeneratorGAM::Setup() {
    bool ret = (numberOfInputSignals == 5u);
    if (ret) {
        for (uint32 i = 0u; (i < (numberOfInputSignals - 1u)) && ret; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            ret = (td == Float32Bit);
            if (ret) {
                uint32 nElements;
                GetSignalNumberOfElements(InputSignals, i, nElements);
                ret = (nElements == numberOfOutputSignals);
                if (!ret) {
                    REPORT_ERROR("ErrorManagement::FatalError", "Number of elements of input signal %d shall be equal to number of outputs", i);
                }
            } else {
                REPORT_ERROR("ErrorManagement::FatalError", "The type of input signal %d must be float32", i);
            }
        }
        TypeDescriptor td = GetSignalType(InputSignals, (numberOfInputSignals - 1u));
        ret = (td == UnsignedInteger64Bit);
        if (!ret) {
            REPORT_ERROR("ErrorManagement::FatalError", "The type of input signal %d must be uint64", (numberOfInputSignals - 1u));
        }
    } else {
        REPORT_ERROR("ErrorManagement::FatalError", "Number of input signals should be equal to 5");
    }
    if (ret) {
        for (uint32 i = 0u; (i < numberOfOutputSignals) && ret; i++) {
            TypeDescriptor outTd = GetSignalType(OutputSignals, i);
            ret = (outTd == UnsignedInteger16Bit);
            if (!ret) {
                REPORT_ERROR("ErrorManagement::FatalError", "The type of output signal %d must be uint16", i);
            }
        }
    }
    if (ret) {
        numberOfSineElements = 1u;
        if (sampleTime > 0u) {
            uint32 numberOfTimeElements = 0u;
            GetSignalNumberOfElements(InputSignals, (numberOfInputSignals - 1u), numberOfTimeElements);
            GetSignalNumberOfElements(OutputSignals, 0u, numberOfSineElements);
            ret = (numberOfTimeElements == numberOfSineElements);
            if (ret) {
                REPORT_ERROR("ErrorManagement::FatalError", "If Period not defined, the Timestamp (input signal 3) must have same #elements of Sine (output signals)");
            }
        }
    }
    if (ret) {
        amplitude = (float32*) GetInputSignalMemory(0u);
        phase = (float32*) GetInputSignalMemory(1u);
        offset = (float32*) GetInputSignalMemory(2u);
        frequency = (float32*) GetInputSignalMemory(3u);
        timestampUs = (uint64*) GetInputSignalMemory(4u);

        sine = (uint16*) GetOutputSignalMemory(0u);
    }
    return ret;
}

bool SineGeneratorGAM::Execute() {
    for (uint32 n = 0; n < numberOfOutputSignals; n++) {
        uint32 timeUs = timestampUs[0];
        for (uint32 i = 0u; i < numberOfSineElements; i++) {
            float32 timeF = ((float32) timeUs) / 1e6;
            float32 angle = (2 * FastMath::PI * (frequency[n]) * timeF) + (phase[n]);
            float32 sineFlt = ((amplitude[n]) * FastMath::Sin(angle)) + (offset[n]);
            float32 factor = (sineFlt - inputMin) / ((float32) (inputMax - inputMin));
            sine[(n * numberOfSineElements) + i] = (uint16) (((outputMax - outputMin) * factor) + outputMin);
            timeUs += sampleTime;
        }
    }
    return true;
}

CLASS_REGISTER(SineGeneratorGAM, "1.0")
