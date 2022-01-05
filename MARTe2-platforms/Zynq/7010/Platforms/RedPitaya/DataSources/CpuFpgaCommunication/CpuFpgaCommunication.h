/*
 * CpuFpgaCommunication.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef CPUFPGACOMMUNICATION_H_
#define CPUFPGACOMMUNICATION_H_

#include "GenericStreamDriver.h"

class CpuFpgaCommunication: public GenericStreamDriver {
public:

	CLASS_REGISTER_DECLARATION()

	CpuFpgaCommunication();
	virtual ~CpuFpgaCommunication();

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

	virtual void Read(void * bufferToFill, uint32 &sizeToRead);

	virtual void Write(void * bufferToFlush, uint32 &sizeToWrite);

	virtual bool Synchronise();

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

    virtual uint32 GetReadBufferPointer();

    virtual uint32 GetWriteBufferPointer();


protected:
	uint32 *readFromMem;
	uint32 *writeToMem;
	uint32 readFromSize;
	uint32 writeToSize;
	uint32 numberOfReadBuffers;
	uint32 numberOfWriteBuffers;
	int8 isReadSync;
	uint32 *syncReadReg;
	uint32 syncReadBitMask;
	int8 isWriteSync;
	uint32 *syncWriteReg;
	uint32 syncWriteBitMask;
	uint32 filledBuffIndex;
	uint32 fillingBuffIndex;
	uint32 toBeFlushedBuffIndex;
	uint32 flushingBuffIndex;
	uint8 useBufferPointer;

};

#endif /* CPUFPGACOMMUNICATION_H_ */
