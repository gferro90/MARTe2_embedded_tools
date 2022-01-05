/*
 * WaveformGenerator.h
 *
 *  Created on: Feb 15, 2018
 *      Author: codac-dev
 */

#ifndef WAVEFORMGENERATOR_H_
#define WAVEFORMGENERATOR_H_

#include "MemoryProvider.h"

using namespace MARTe;

class WaveformGenerator: public MemoryProvider {
public:
	CLASS_REGISTER_DECLARATION()

	WaveformGenerator();
	virtual ~WaveformGenerator();
	virtual bool SetConfiguredDatabase(StructuredDataI & data);
	virtual bool Initialise(StructuredDataI &data);

	float factor;
	float offset;
};

#endif /* WAVEFORMGENERATOR_H_ */
