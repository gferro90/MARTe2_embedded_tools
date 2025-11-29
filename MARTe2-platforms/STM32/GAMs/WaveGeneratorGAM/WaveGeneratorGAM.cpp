/**
 * @file WaveGeneratorGAM.cpp
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
#include "WaveGeneratorGAM.h"
#include "FastMath.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
WaveGeneratorGAM::WaveGeneratorGAM() :
        GAM() {
    time = NULL;
    value = NULL;
    period = NULL;
    pushpull = NULL;
    interpolation = NULL;
    timestampUs = NULL;
    wave = NULL;
    sampleTime = 0u;
    numberOfWaveElements = 1u;
    outputMin = 0u;
    outputMax = 4095u;
    inputMin = -1.;
    inputMax = 1.;
    pushpull_1=0;
    nPoints=0u;
    currentPointIdx=0u;
    nextPointIdx=0u;
}

WaveGeneratorGAM::~WaveGeneratorGAM() {

}

bool WaveGeneratorGAM::Initialise(StructuredDataI &data) {
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

bool WaveGeneratorGAM::Setup() {
    bool ret = (numberOfInputSignals == 6u);
    if (ret) {
        for (uint32 i = 0u; (i < (numberOfInputSignals - 1u)) && ret; i++) {
            TypeDescriptor td = GetSignalType(InputSignals, i);
            if (i == 0u) {
                ret = (td == UnsignedInteger32Bit);
            } else if (i == 1u) {
                ret = (td == Float32Bit);
            } else if (i == 2u) {
                ret = (td == UnsignedInteger32Bit);
            } else if (i == 3u) {
                ret = (td == SignedInteger8Bit);
            } else if (i == 4u) {
                ret = (td == UnsignedInteger8Bit);
            }
            if (ret) {
                uint32 nElements;
                GetSignalNumberOfElements(InputSignals, i, nElements);
                ret = (nElements == numberOfOutputSignals);
                if (!ret) {
                    REPORT_ERROR("ErrorManagement::FatalError", "Number of elements of input signal %d shall be equal to number of outputs", i);
                }
            } else {
                REPORT_ERROR("ErrorManagement::FatalError", "The type of input signal %d must be %s", TypeDescriptor::GetTypeNameFromTypeDescriptor(td));
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
        numberOfWaveElements = 1u;
        if (sampleTime > 0u) {
            uint32 numberOfTimeElements = 0u;
            GetSignalNumberOfElements(InputSignals, (numberOfInputSignals - 1u), numberOfTimeElements);
            GetSignalNumberOfElements(OutputSignals, 0u, numberOfWaveElements);
            ret = (numberOfTimeElements == numberOfWaveElements);
            if (ret) {
                REPORT_ERROR("ErrorManagement::FatalError", "If SampleTimeUs not defined, the Timestamp (input signal 3) must have same #elements of Sine (output signals)");
            }
        }
    }
    if (ret) {
        time = (uint32*) GetInputSignalMemory(0u);
        value = (float32*) GetInputSignalMemory(1u);
        period = (uint32*) GetInputSignalMemory(2u);
        interpolation = (uint8*) GetInputSignalMemory(3u);
        pushpull = (int8*) GetInputSignalMemory(4u);
        timestampUs = (uint64*) GetInputSignalMemory(5u);

        wave = (uint16*) GetOutputSignalMemory(0u);
    }
    return ret;
}

bool WaveGeneratorGAM::Execute() {
    if (((*pushpull) > 0) && (pushpull_1 == 0)) {
        //add new value
        AddPoint();
    }
    if (((*pushpull) < 0) && (pushpull_1 == 0)) {
        //remove existent value
        RemovePoint();
    }

    for (uint32 n = 0; n < numberOfOutputSignals; n++) {
        uint32 timeUs = (uint32) (timestampUs[0] % (*period));
        for (uint32 i = 0u; i < numberOfWaveElements; i++) {
            if (timeUs >= times[nextPointIdx]) {
                currentPointIdx = nextPointIdx;
                nextPointIdx = (currentPointIdx + 1u) % nPoints;
            }
            if ((*interpolation) == 0u) {
                wave[i] = points[currentPointIdx];
            } else if ((*interpolation) == 1u) {
                uint32 deltaX = times[currentPointIdx] - times[nextPointIdx];
                uint16 deltaY = points[currentPointIdx] - points[nextPointIdx];
                float32 step = deltaY / ((float32) deltaX);
                wave[i] = points[currentPointIdx] + step * (timeUs - times[currentPointIdx]);
            }
            timeUs += sampleTime;
        }
    }
    pushpull_1 = (*pushpull);
    return true;
}

void WaveGeneratorGAM::AddPoint() {
    uint8 max = nPoints;
    uint8 min = 0u;
    uint8 index = ((max - min) / 2);
    bool add = true;
    if (nPoints < 128) {
        while (max > min) {
            index = ((max - min) / 2);
            if ((*time) > times[index]) {
                min = index;
            } else if ((*time) < times[index]) {
                max = index;
            } else {
                //do nothing, other value at same time
                add = false;
                break;
            }
        }
        if (add) {
            MemoryOperationsHelper::Move(times[index + 1u], times[index], (nPoints - index));
            MemoryOperationsHelper::Move(points[index + 1u], points[index], (nPoints - index));

            times[index] = (*time);
            float32 factor = ((*value) - inputMin) / ((float32) (inputMax - inputMin));
            points[index] = (uint16) (((outputMax - outputMin) * factor) + outputMin);
            nPoints++;
        }
    }
}

void WaveGeneratorGAM::RemovePoint() {
    uint8 max = nPoints;
    uint8 min = 0u;
    uint8 index = ((max - min) / 2);
    bool found = false;
    if (nPoints > 0u) {
        while (max > min) {
            index = ((max - min) / 2);
            if ((*time) > times[index]) {
                min = index;
            } else if ((*time) < times[index]) {
                max = index;
            } else {
                //do nothing, other value at same time
                found = true;
                break;
            }
        }
        if (found) {
            nPoints--;
            MemoryOperationsHelper::Move(times[index], times[index+1], (nPoints - index));
            MemoryOperationsHelper::Move(points[index], points[index+1], (nPoints - index));
        }
    }
}


CLASS_REGISTER(WaveGeneratorGAM, "1.0")
