/*
 * MemoryProvider.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: gferro
 */

#include "MemoryProvider.h"

#include "MemoryProvider.h"

#include "AdvancedErrorManagement.h"
#include "GenericStreamReader.h"
#include "GenericStreamWriter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryProvider::MemoryProvider() :
		GAMDataSource() {
	bufferSize = NULL;
	buffer = NULL;
	nOfSignals = 0;
}

MemoryProvider::~MemoryProvider() {
	if (bufferSize != NULL) {
		delete[] bufferSize;
		bufferSize = NULL;
	}
	if (buffer != NULL) {
		for (uint32 i = 0; i < nOfSignals; i++) {
			if (buffer[i] != NULL) {
				delete[] buffer[i];
				buffer[i] = NULL;
			}
		}
		delete[] buffer;
		buffer = NULL;
	}
	if (bufferCopy != NULL) {
		delete[] bufferCopy;
		bufferCopy = NULL;
	}
}

bool MemoryProvider::Initialise(StructuredDataI &data) {
	bool ret = GAMDataSource::Initialise(data);

	return ret;
}

bool MemoryProvider::AllocateMemory() {
	return true;
}

uint32 MemoryProvider::GetNumberOfMemoryBuffers() {
	return 1u;
}

bool MemoryProvider::GetSignalMemoryBuffer(const uint32 signalIdx,
		const uint32 bufferIdx, void *&signalAddress) {
	bool ret = (buffer != NULL);
	if (ret) {
		signalAddress = reinterpret_cast<void *>(&bufferCopy[signalIdx]);
		REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
				"Address=%x %x provided %d", (uint32)(signalAddress),
				*(uint32*) signalAddress, signalIdx);

	}
	return ret;
}

bool MemoryProvider::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = DataSourceI::SetConfiguredDatabase(data);
	/*if (ret) {
	 ret = signalsDatabase.MoveRelative("Signals");
	 if (!ret) {
	 REPORT_ERROR(ErrorManagement::InitialisationError,
	 "Failed move to \"Signals\" node");
	 }
	 }*/
	if (ret) {
		//check for buffer size and data type within the
		//signals configuration and allocate the memory
		nOfSignals = GetNumberOfSignals();
		bufferSize = new uint32[nOfSignals];
		buffer = new char8*[nOfSignals];
		bufferCopy = new char8*[nOfSignals];
		if (ret) {
			for (uint32 n = 0u; (n < nOfSignals) && (ret); n++) {
				const char8* childName = signalsDatabase.GetChildName(n);
				ret = signalsDatabase.MoveRelative(childName);
				uint32 signalBufferSize;
				if (ret) {
					ret = signalsDatabase.Read("BufferSize", signalBufferSize);
					REPORT_ERROR_PARAMETERS(
							ErrorManagement::InitialisationError,
							"BufferSize=%d for signal %s", signalBufferSize,
							childName);

				} else {
					REPORT_ERROR_PARAMETERS(
							ErrorManagement::InitialisationError,
							"Failed move to child %s", childName);
				}
				if (ret) {
					bufferSize[n] = signalBufferSize;
					buffer[n] = new char8[signalBufferSize];
					MemoryOperationsHelper::Set(buffer[n], '\0', bufferSize[n]);
					uint32 addr = (uint32)(buffer[n]);
					REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
							"Address of %s=%x", childName, addr);

					bufferCopy[n] = buffer[n];
					ret = signalsDatabase.MoveToAncestor(1u);
				}
			}
		}
		if (ret) {
			ret = signalsDatabase.MoveToAncestor(1u);
		}
	}
	return ret;
}

uint32 MemoryProvider::GetSignalBufferSize(uint32 signalIdx) const {
	return bufferSize[signalIdx];
}

void *MemoryProvider::GetSignalBufferPointer(uint32 signalIdx) const {
	return bufferCopy[signalIdx];
}

bool MemoryProvider::PrepareNextState(const char8 * const currentStateName,
		const char8 * const nextStateName) {
	return true;
}

CLASS_REGISTER(MemoryProvider, "1.0")
