/*
 * CpuFpgaCommunication.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#include "CpuFpgaCommunication.h"
#include "AdvancedErrorManagement.h"
#include "CpuFpgaCommunicationReader.h"
#include "CpuFpgaCommunicationWriter.h"

#include <string.h>

CpuFpgaCommunication::CpuFpgaCommunication() :
		GenericStreamDriver() {
	readFromMem = NULL;
	writeToMem = NULL;
	readFromSize = 0;
	writeToSize = 0;
	numberOfReadBuffers = 0;
	numberOfWriteBuffers = 0;
	isReadSync = 0;
	syncReadReg = NULL;
	syncReadBitMask = 0;
	isWriteSync = 0;
	syncWriteReg = NULL;
	syncWriteBitMask = 0;
	filledBuffIndex = 0;
	fillingBuffIndex = 0;
	toBeFlushedBuffIndex = 0;
	flushingBuffIndex = 0;
	useBufferPointer = 0;

}

CpuFpgaCommunication::~CpuFpgaCommunication() {
	if (syncReadReg != NULL) {
		delete[] syncReadReg;
		syncReadReg = NULL;
	}
	if (syncWriteReg != NULL) {
		delete[] syncWriteReg;
		syncWriteReg = NULL;
	}
	if (readFromMem != NULL) {
		delete[] readFromMem;
		readFromMem = NULL;
	}
	if (writeToMem != NULL) {
		delete[] writeToMem;
		writeToMem = NULL;
	}

}

bool CpuFpgaCommunication::GetSignalMemoryBuffer(const uint32 signalIdx,
		const uint32 bufferIdx, void *&signalAddress) {
	if (allocateBuffer) {
		GAMDataSource::GetSignalMemoryBuffer(signalIdx, bufferIdx,
				signalAddress);
	} else {
		StreamString signalName;
		GetSignalName(0u, signalName);
		uint32 numberOfConsumers = 0;
		GetSignalNumberOfConsumers(0u, currentStateName.Buffer(), numberOfConsumers);
		//if consumer the read memory
		if (numberOfConsumers > 0) {
			if (readFromMem != NULL) {
				signalAddress = (void*) 0xFFFFFFFF;//(readFromMem);
			} else {
				//TODO error
			}
		}
		uint32 numberOfProducers = 0;
		GetSignalNumberOfProducers(0u, currentStateName.Buffer(), numberOfProducers);
		//if producer the write memory
		if (numberOfProducers > 0) {
			if (writeToMem != NULL) {
				signalAddress = (void*) 0xFFFFFFFF;//(writeToMem);
			} else {
				//TODO error
			}
		}

	}
	return true;
}

bool CpuFpgaCommunication::Initialise(StructuredDataI &data) {
	bool ret = GenericStreamDriver::Initialise(data);
	if (ret) {

		if (!data.Read("NumberOfReadBuffers", numberOfReadBuffers)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"numberOfReadBuffers not set: using default 1");
			numberOfReadBuffers = 1;
		}

		if (!data.Read("NumberOfWriteBuffers", numberOfWriteBuffers)) {
			REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
					"numberOfWriteBuffers not set: using default 1");
			numberOfWriteBuffers = 1;
		}

		if (!data.Read("ReadFromSize", readFromSize)) {
			readFromSize = 0;
			REPORT_ERROR(ErrorManagement::Warning,
					"ReadFromSize Not Found: using default 0");
		}

		if (!data.Read("WriteToSize", writeToSize)) {
			writeToSize = 0;
			REPORT_ERROR(ErrorManagement::Warning,
					"WriteToSize Not Found: using default 0");
		}

		//read channels
		if (data.Read("IsReadSync", isReadSync)) {
			if (isReadSync >= 0) {
				readFromMem = new uint32[numberOfReadBuffers];
				Vector < uint32
						> readFromMemVec(readFromMem, numberOfReadBuffers);
				if (!data.Read("ReadFromMem", readFromMemVec)) {
					REPORT_ERROR(ErrorManagement::Warning,
							"Error reading ReadFromMem: using default 0");
					for (uint32 i = 0u; i < numberOfReadBuffers; i++) {
						readFromMem[i] = 0;
					}
				}

				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
						"ReadFromMem %x %x", readFromMem[0], readFromMem[1]);

				if (isReadSync > 0) {
					//read channels
					syncReadReg = new uint32[numberOfReadBuffers];
					Vector < uint32
							> syncReadRegVec(syncReadReg, numberOfReadBuffers);
					ret = data.Read("SyncReadRegisters", syncReadRegVec);
					if (ret) {
						if (!data.Read("SyncReadBitMask", syncReadBitMask)) {
							REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
									"SyncReadBitMask not set: using default 0");
							syncReadBitMask = 0;
						}
						REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
								"SyncReadReg %x %x %x", syncReadReg[0],
								syncReadReg[1], syncReadBitMask);

					} else {
						REPORT_ERROR(ErrorManagement::InitialisationError,
								"Error reading SyncReadRegisters");
					}
				}
			}
		}
		if (data.Read("IsWriteSync", isWriteSync)) {
			if (isWriteSync >= 0) {

				writeToMem = new uint32[numberOfReadBuffers];
				Vector < uint32
						> writeToMemVec(writeToMem, numberOfReadBuffers);
				if (!data.Read("WriteToMem", writeToMemVec)) {
					REPORT_ERROR(ErrorManagement::Warning,
							"Error reding WriteToMem: using default 0");
					for (uint32 i = 0u; i < numberOfReadBuffers; i++) {
						writeToMem[i] = 0;
					}
				}
				if (isWriteSync > 0) {
					//read channels
					syncWriteReg = new uint32[numberOfReadBuffers];
					Vector < uint32
							> syncWriteRegVec(syncWriteReg,
									numberOfReadBuffers);
					ret = data.Read("SyncWriteRegisters", syncWriteRegVec);
					if (ret) {
						if (!data.Read("SyncWriteBitMask", syncWriteBitMask)) {
							REPORT_ERROR_STATIC_0(ErrorManagement::Warning,
									"SyncWriteBitMask not set: using default 0");
							syncWriteBitMask = 0;
						}

					} else {
						REPORT_ERROR(ErrorManagement::InitialisationError,
								"Error reading SyncWriteRegisters");
					}
				}
			}
		}

		if (data.Read("UseBufferPointer", useBufferPointer)) {
			if (!allocateBuffer) {
				//TODO warning
				useBufferPointer = 1u;
			}
		} else {
			useBufferPointer = 1u;
			//TODO warning
		}

	}

	return ret;
}

const char8 *CpuFpgaCommunication::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if (direction == InputSignals) {
		return "CpuFpgaCommunicationReader";
	}
	if (direction == OutputSignals) {
		return "CpuFpgaCommunicationWriter";
	}
	return "";
}

bool CpuFpgaCommunication::GetInputBrokers(ReferenceContainer &inputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<CpuFpgaCommunicationReader> broker("CpuFpgaCommunicationReader");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = inputBrokers.Insert(broker);
	}
	return ret;
}

bool CpuFpgaCommunication::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<CpuFpgaCommunicationWriter> broker("CpuFpgaCommunicationWriter");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = outputBrokers.Insert(broker);
	}
	return ret;
}

void CpuFpgaCommunication::Read(void *bufferToFill, uint32 &sizeToRead) {
	Synchronise();
	if (useBufferPointer) {
		*(uint32*) (bufferToFill) = readFromMem[filledBuffIndex];
	} else {
		MemoryOperationsHelper::Copy(bufferToFill,
				(void*) readFromMem[filledBuffIndex], sizeToRead);
	}
}

void CpuFpgaCommunication::Write(void *bufferToFlush, uint32 &sizeToWrite) {
	Synchronise();
	if (useBufferPointer) {
		writeToMem[toBeFlushedBuffIndex] = *(uint32*) bufferToFlush;
	} else {
		MemoryOperationsHelper::Copy((void*) writeToMem[toBeFlushedBuffIndex],
				bufferToFlush, sizeToWrite);
	}
}

bool CpuFpgaCommunication::Synchronise() {
	if (isReadSync) {
		//TODO If the sync bit is already asserted maybe a warning should be
		//generated to say that the execution is too slow
		//wait the sync bit to be asserted
		while (((*(volatile uint32*) (syncReadReg[fillingBuffIndex])) & syncReadBitMask)
				!= syncReadBitMask) {

		}
		//de-assert the sync bit
		filledBuffIndex = fillingBuffIndex;
		fillingBuffIndex++;
		fillingBuffIndex %= numberOfReadBuffers;
		(*(volatile uint32*) (syncReadReg[fillingBuffIndex])) &= (~syncReadBitMask);
		(*(volatile uint32*) (syncReadReg[filledBuffIndex])) &= (~syncReadBitMask);

		/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "indexes %d %d",
				fillingBuffIndex, filledBuffIndex);*/
	}
	else{
		filledBuffIndex = fillingBuffIndex;
		fillingBuffIndex++;
		fillingBuffIndex %= numberOfReadBuffers;
	}
	return true;
	//TODO How to sync on eventual write?? (need an argument)

}

bool CpuFpgaCommunication::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = GenericStreamDriver::SetConfiguredDatabase(data);
	/*REPORT_ERROR(ErrorManagement::InitialisationError,
	 "Here1");*/
	if (ret) {
		//Only one signal is allowed!!
		ret = (GetNumberOfSignals() == 1);
		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"Only one signal is allowed");		}

		ret = (GetSignalType(0u)
				== TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"The signal type has to be uint32");
		}

	}
	return ret;
}

bool CpuFpgaCommunication::PrepareNextState(
		const char8 * const currentStateNameIn,
		const char8 * const nextStateNameIn) {
	bool ret = GenericStreamDriver::PrepareNextState(currentStateNameIn,
			nextStateNameIn);
	if (ret) {
		if (isReadSync) {
			for(uint32 i=0; i<numberOfReadBuffers; i++){
				MemoryOperationsHelper::Set((void*)(readFromMem[i]), '\0', readFromSize);
			}

			(*(volatile uint32*) (syncReadReg[0])) &= ~syncReadBitMask;
			if (numberOfReadBuffers > 0) {
				(*(volatile uint32*) (syncReadReg[1])) &= ~syncReadBitMask;
			}

		}
	}
	return ret;
}

uint32 CpuFpgaCommunication::GetReadBufferPointer() {
	//uint32 x=readFromMem[filledBuffIndex];
	//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Giving %d %d", x, filledBuffIndex);
	return readFromMem[filledBuffIndex];
}

uint32 CpuFpgaCommunication::GetWriteBufferPointer() {
	return writeToMem[toBeFlushedBuffIndex];
}



CLASS_REGISTER(CpuFpgaCommunication, "1.0")
