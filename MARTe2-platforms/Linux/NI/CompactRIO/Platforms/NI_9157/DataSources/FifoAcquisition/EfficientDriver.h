/*
 * EfficientDriver.h
 *
 *  Created on: Feb 14, 2018
 *      Author: codac-dev
 */

#ifndef EFFICIENTDRIVER_H_
#define EFFICIENTDRIVER_H_

#include "DataSourceI.h"

using namespace MARTe;

class EfficientDriver: public DataSourceI {
public:

	EfficientDriver();
	virtual ~EfficientDriver();

	virtual bool AllocateMemory();

	virtual uint32 GetNumberOfMemoryBuffers();


	virtual bool GetSignalMemoryBuffer(const uint32 signalIdx,
			const uint32 bufferIdx, void *&signalAddress);

	virtual bool Initialise(StructuredDataI &data);

	virtual const char8 *GetBrokerName(StructuredDataI &data,
			const SignalDirection direction);

	virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
			const char8* const functionName, void * const gamMemPtr);

	virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
			const char8* const functionName, void * const gamMemPtr);

	virtual bool PrepareNextState(const char8 * const currentStateNameIn,
			const char8 * const nextStateNameIn);

	virtual bool Synchronise()=0;

	virtual bool SetConfiguredDatabase(StructuredDataI & data);

	virtual uint64 GetReadBufferPointer();

	virtual uint64 GetWriteBufferPointer();

	virtual bool IsInput();

protected:
	uint8* readMem;
	uint8* writeMem;
	uint32 readBufferSize;
	uint32 writeBufferSize;
	uint32 numberOfReadBuffers;
	uint32 numberOfWriteBuffers;
	uint32 readBufferIndex;
	uint32 writeBufferIndex;
	uint8 isInput;
	uint32 numberOfSignals;

};

#endif /* MARTE2_EMBEDDED_PROJECTS_LINUX_NI_9157_LOOPTIMERACQ_MARTE_DATASOURCES_EFFICIENTDRIVER_H_ */
