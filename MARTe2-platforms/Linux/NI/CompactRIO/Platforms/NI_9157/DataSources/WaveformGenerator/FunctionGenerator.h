/*
 * FunctionGenerator.h
 *
 *  Created on: Feb 16, 2018
 *      Author: codac-dev
 */

#ifndef FUNCTIONGENERATOR_H_
#define FUNCTIONGENERATOR_H_

#include "ReferenceContainer.h"

using namespace MARTe;

class FunctionGenerator: public ReferenceContainer {
public:
	FunctionGenerator();
	virtual ~FunctionGenerator();
	virtual bool FillMem(uint16* mem, uint32 bufferSize, float factor, float offset)=0;

};

#endif /* FUNCTIONGENERATOR_H_ */
