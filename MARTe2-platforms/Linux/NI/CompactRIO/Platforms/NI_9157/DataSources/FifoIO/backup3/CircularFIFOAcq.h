/*
 * CircularFIFOAcq.h
 *
 *  Created on: Feb 21, 2018
 *      Author: codac-dev
 */

#ifndef CIRCULARFIFOACQ_H_
#define CIRCULARFIFOACQ_H_

#include "CircularBufferAcqDriver.h"
#include "NiDeviceT.h"
using namespace MARTe;

class CircularFIFOAcq: public CircularBufferAcqDriver {
public:
	CLASS_REGISTER_DECLARATION()

	CircularFIFOAcq();
	virtual ~CircularFIFOAcq();
    virtual bool Initialise(StructuredDataI &data);
    virtual bool DriverRead(char8 *bufferToFill, uint32 &sizeToRead);
    virtual bool NegotiateWithBroker(uint32 signalIndex,
    		ReferenceT<InputPackager> broker);

	virtual bool PrepareNextState(const char8 * const currentStateName,
			const char8 * const nextStateName);
	virtual ErrorManagement::ErrorType Execute(
			const ExecutionInfo & info);
private:
	uint32 fifoDev;
    uint64 endOfFrame;
    uint8 sampleByteSize;
    ReferenceT<NiDevice> niDeviceBoard;
    NiDeviceTGen *niDevice;
};

#endif /* CIRCULARFIFOACQ_H_ */
