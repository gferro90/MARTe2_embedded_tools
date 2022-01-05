/*
 * MemoryProvider.h
 *
 *  Created on: Jan 25, 2018
 *      Author: gferro
 */

#ifndef MEMORYPROVIDER_H_
#define MEMORYPROVIDER_H_

#include "GAMDataSource.h"

using namespace MARTe;

class MemoryProvider: public GAMDataSource {
public:

	CLASS_REGISTER_DECLARATION()

	MemoryProvider();
	virtual ~MemoryProvider();

    bool AllocateMemory();

    uint32 GetNumberOfMemoryBuffers();

    bool GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress);

	virtual bool Initialise(StructuredDataI &data);

	virtual bool SetConfiguredDatabase(StructuredDataI & data);

	uint32 GetSignalBufferSize(uint32 signalIdx) const;

	void *GetSignalBufferPointer(uint32 signalIdx) const;

	virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);
protected:
	uint32 *bufferSize;
	char8 **buffer;
	char8 **bufferCopy;

	uint32 nOfSignals;

};

#endif /* MEMORYPROVIDER_H_ */
