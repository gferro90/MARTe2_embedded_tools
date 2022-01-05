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
#include "NiDeviceT.h"

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
	ReferenceT<NiDevice> niDeviceBoard;
	uint32 fifoDev;
	uint8 sampleByteSize;
	uint32 cycleTimeUs;
	int32 inputMaxVal;
	int32 inputResolution;
	int32 outputMaxVal;
	int32 outputResolution;
	uint32 inputTriggerVar;
	uint32 outputTriggerVar;
	uint8 isTrigger;
	NiDeviceTGen *niDevice;
	uint32 timeout;
	uint8 runNi;
};

#endif /* FIFOACQUISITION_H_ */
