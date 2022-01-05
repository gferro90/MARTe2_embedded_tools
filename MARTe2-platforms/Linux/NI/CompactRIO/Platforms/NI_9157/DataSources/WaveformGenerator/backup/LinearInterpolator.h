/*
 * LinearInterpolator.h
 *
 *  Created on: Feb 19, 2018
 *      Author: codac-dev
 */

#ifndef LINEARINTERPOLATOR_H_
#define LINEARINTERPOLATOR_H_

#include "FunctionGenerator.h"

using namespace MARTe;

class LinearInterpolator: public FunctionGenerator {
public:
	CLASS_REGISTER_DECLARATION()

	LinearInterpolator();
	virtual ~LinearInterpolator();
    virtual bool Initialise(StructuredDataI &data);
	virtual bool FillMem(int32* mem, uint32 bufferSize, float factor, float offset);

protected:
	float *point;
	float *timeFactor;
	uint32 numberOfPoints;
};

#endif /* LINEARINTERPOLATOR_H_ */
