/*
 * FifoIO.h
 *
 *  Created on: Feb 14, 2018
 *      Author: codac-dev
 */

#ifndef FIFOACQUISITION_H_
#define FIFOACQUISITION_H_

#include "EfficientDriver.h"
#include "SearchLabviewGen.h"
#include "NiDevice.h"

using namespace MARTe;

class FifoIO: public EfficientDriver {
public:
	CLASS_REGISTER_DECLARATION()

FifoIO();
	virtual ~FifoIO();
	virtual bool Initialise(StructuredDataI & data);

	bool PrepareNextState(const char8 * const currentStateName,
			const char8 * const nextStateName);
	virtual bool Synchronise();

protected:
	ReferenceT<NiDevice> niDevice;
	NiFpga_Session session;
	uint32 fifoDev;
	int32 store;
	uint8 sampleByteSize;
	uint32 cycleTimeUs;
	int32 inputMaxVal;
	int32 inputResolution;
	int32 outputMaxVal;
	int32 outputResolution;
	uint32 inputTriggerVar;
	uint32 outputTriggerVar;
	uint8 isTrigger;
};

#endif /* FIFOACQUISITION_H_ */
