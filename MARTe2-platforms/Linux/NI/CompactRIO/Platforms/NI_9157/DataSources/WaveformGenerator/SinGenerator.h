/*
 * SinGenerator.h
 *
 *  Created on: Feb 16, 2018
 *      Author: codac-dev
 */

#ifndef SINGENERATOR_H_
#define SINGENERATOR_H_

#include "FunctionGenerator.h"

using namespace MARTe;

class SinGenerator: public FunctionGenerator {
public:
	CLASS_REGISTER_DECLARATION()

	SinGenerator();
	virtual ~SinGenerator();
    virtual bool Initialise(StructuredDataI &data);
	virtual bool FillMem(uint16* mem, uint32 bufferSize, float factor, float offset);

protected:
	float *frequency;
	float *amplitude;
	float *phase;
	uint32 numberOfWaves;
};

#endif /* SINGENERATOR_H_ */
