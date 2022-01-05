/*
 * HwCounter.cpp
 *
 *  Created on: Jan 24, 2018
 *      Author: gferro
 */

#include "HwCounter.h"
#include "AdvancedErrorManagement.h"
#include "GenericStreamReader.h"

HwCounter::HwCounter() {
	// TODO Auto-generated constructor stub
	counterFrequency = 0;
}

HwCounter::~HwCounter() {
	// TODO Auto-generated destructor stub
}

bool HwCounter::Initialise(StructuredDataI &data) {
	bool ret = CpuFpgaCommunication::Initialise(data);
	if (ret) {
		ret = data.Read("CounterFrequency", counterFrequency);
		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"CounterFrequency not set");
		}
		if (useBufferPointer) {
			REPORT_ERROR(ErrorManagement::Warning,
					"UseBufferPointer must be false");
			useBufferPointer = 0;
		}
		if (!allocateBuffer) {
			REPORT_ERROR(ErrorManagement::Warning,
					"AllocateBuffer must be true");
			allocateBuffer = 1;
		}
	}
	return ret;
}

const char8 *HwCounter::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if (direction == InputSignals) {
		return "GenericStreamReader";
	}
	if (direction == OutputSignals) {
		return "";
	}
	return "";
}

bool HwCounter::GetInputBrokers(ReferenceContainer &inputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<GenericStreamReader> broker("GenericStreamReader");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = inputBrokers.Insert(broker);
	}
	return ret;
}

bool HwCounter::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	return false;
}

bool HwCounter::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = GenericStreamDriver::SetConfiguredDatabase(data);
	/*REPORT_ERROR(ErrorManagement::InitialisationError,
	 "Here1");*/
	if (ret) {
		//Only one signal is allowed!!
		ret = (GetNumberOfSignals() == 1);
		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"Only one signal is allowed");

		}

		ret = (GetSignalType(0u)
				== TypeDescriptor::GetTypeDescriptorFromTypeName("uint64"));
		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"The signal type has to be uint64");
		}

	}
	return ret;
}

void HwCounter::Read(void *bufferToFill, uint32 &sizeToRead) {
	uint32 counter = *(((uint32*) (readFromMem[filledBuffIndex])) + 2);
	if (storeCounter > counter) {
		nOverflows++;
	}
	storeCounter = counter;
	uint64 timer = (((0xFFFFFFFF * nOverflows) + counter) / counterFrequency)
			* 1e6;

	MemoryOperationsHelper::Copy(bufferToFill, (void*) (&timer),
			sizeof(uint64));
}

void HwCounter::Write(void *bufferToFlush, uint32 &sizeToWrite) {

}

bool HwCounter::PrepareNextState(const char8 * const currentStateNameIn,
		const char8 * const nextStateNameIn) {
	bool ret = GenericStreamDriver::PrepareNextState(currentStateNameIn,
			nextStateNameIn);
	if (ret) {
		Restart();
	}
	return ret;
}

void HwCounter::Start() {
	*(uint32*) (readFromMem[filledBuffIndex]) = 1;
}

void HwCounter::Stop() {
	*(uint32*) (readFromMem[filledBuffIndex]) = 0;
}

void HwCounter::Reset() {
	*(uint32*) (readFromMem[filledBuffIndex]) = 2;
}

void HwCounter::Restart() {
	*(uint32*) (readFromMem[filledBuffIndex]) = 0;
	*(uint32*) (readFromMem[filledBuffIndex]) = 2;
	*(uint32*) (readFromMem[filledBuffIndex]) = 1;
}

uint64 HwCounter::Counter() {
	return (uint64)(*((uint32*) (readFromMem[filledBuffIndex]) + 2));
}

CLASS_REGISTER(HwCounter, "1.0")
