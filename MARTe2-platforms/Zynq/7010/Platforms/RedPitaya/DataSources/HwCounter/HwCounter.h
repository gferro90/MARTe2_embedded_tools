/*
 * HwCounter.h
 *
 *  Created on: Jan 24, 2018
 *      Author: gferro
 */

#ifndef HWCOUNTER_H_
#define HWCOUNTER_H_

#include "CpuFpgaCommunication.h"

using namespace MARTe;

class HwCounter: public CpuFpgaCommunication {
public:
	CLASS_REGISTER_DECLARATION()

	HwCounter();
	virtual ~HwCounter();

	virtual bool Initialise(StructuredDataI &data);

	virtual const char8 *GetBrokerName(StructuredDataI &data,
			const SignalDirection direction);

	virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
			const char8* const functionName, void * const gamMemPtr);

	virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
			const char8* const functionName, void * const gamMemPtr);

	virtual bool SetConfiguredDatabase(StructuredDataI & data);

	virtual void Read(void * bufferToFill, uint32 &sizeToRead);

	virtual void Write(void * bufferToFlush, uint32 &sizeToWrite);

	virtual bool PrepareNextState(const char8 * const currentStateNameIn,
			const char8 * const nextStateNameIn);

	virtual void Start();

	virtual void Stop();

	virtual void Reset();

	virtual void Restart();

	virtual uint64 Counter();

private:
	float32 counterFrequency;
	uint8 nOverflows;
	uint64 storeCounter;
};

#endif /* HWCOUNTER_H_ */

