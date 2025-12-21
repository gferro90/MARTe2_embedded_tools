/**
 * @file WaveGeneratorGAM.h
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

#ifndef WAVEGENERATOR_H_
#define WAVEGENERATOR_H_

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
 * @brief This GAM generates waveforms
 *
 * @
 */
class WaveGeneratorGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    WaveGeneratorGAM();

    /**
     * @brief Destructor
     */
    virtual ~WaveGeneratorGAM();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool Execute();

    virtual bool Setup();

private:

    void AddPoint();

    void RemovePoint();

    uint32 *time;

    float32 *value;

    uint64 *timestampUs;

    uint32 *period;

    int8 *pushpull;

    uint8 *interpolation;

    uint16 *wave;

    uint32 sampleTime;

    uint32 numberOfWaveElements;

    float32 inputMin;

    float32 inputMax;

    uint16 outputMin;

    uint16 outputMax;

    int8 pushpull_1;

    uint16 points[128];

    uint32 times[128];

    uint8 nPoints;

    uint8 currentPointIdx;

    uint8 nextPointIdx;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SINEGENERATOR_H_ */

