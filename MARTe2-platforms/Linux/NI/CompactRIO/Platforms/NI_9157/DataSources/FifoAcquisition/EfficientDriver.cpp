/*
 * EfficientDriver.cpp
 *
 *  Created on: Feb 14, 2018
 *      Author: codac-dev
 */

#include "EfficientDriver.h"
#include "AdvancedErrorManagement.h"
#include "EfficientReader.h"
#include "EfficientWriter.h"

EfficientDriver::EfficientDriver() {
	// TODO Auto-generated constructor stub

	readMem = NULL;
	writeMem=NULL;
	readBufferSize=0;
	writeBufferSize=0;
	numberOfReadBuffers=0;
	numberOfWriteBuffers=0;
	readBufferIndex=0;
	writeBufferIndex=0;
	numberOfSignals=0;
}

EfficientDriver::~EfficientDriver() {
	// TODO Auto-generated destructor stub
	if (readMem != NULL) {
		delete[] readMem;
	}
	if(writeMem!=NULL) {
		delete[] writeMem;
	}
}

bool EfficientDriver::AllocateMemory(){
	return true;
}

uint32 EfficientDriver::GetNumberOfMemoryBuffers(){
	return 1u;
}


bool EfficientDriver::GetSignalMemoryBuffer(const uint32 signalIdx,
		const uint32 bufferIdx, void *&signalAddress) {
	signalAddress=NULL;
	return true;
}

bool EfficientDriver::Initialise(StructuredDataI &data) {
	bool ret = DataSourceI::Initialise(data);
	if (ret) {
		if (!data.Read("NumberOfReadBuffers", numberOfReadBuffers)) {
			numberOfReadBuffers = 0u;
		}
		if (!data.Read("NumberOfWriteBuffers", numberOfWriteBuffers)) {
			numberOfWriteBuffers = 0u;
		}
	}
	return ret;
}

const char8 *EfficientDriver::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if(direction==InputSignals){
		return "EfficientReader";
	}
	if(direction==OutputSignals){
		return "EfficientWriter";
	}
	return "";
}

bool EfficientDriver::GetInputBrokers(ReferenceContainer &inputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<EfficientReader> broker("EfficientReader");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = inputBrokers.Insert(broker);
	}
	return ret;
}

bool EfficientDriver::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<EfficientWriter> broker("EfficientWriter");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = outputBrokers.Insert(broker);
	}
	return ret;
}

bool EfficientDriver::PrepareNextState(const char8 * const currentStateNameIn,
		const char8 * const nextStateNameIn) {
	bool ret = true;
	if(ret){
		uint32 numberOfConsumers=0u;
		GetSignalNumberOfConsumers(0u,nextStateNameIn,numberOfConsumers);
		uint32 numberOfProducers=0u;
		GetSignalNumberOfProducers(0u,nextStateNameIn,numberOfProducers);
		ret=((numberOfConsumers*numberOfProducers)==0u);
		isInput=(numberOfConsumers>0u);
	}
	if (ret) {

		if (readMem != NULL) {
			delete[] readMem;
		}
		if (writeMem != NULL) {
			delete[] writeMem;
		}
		readBufferSize = 0u;
		writeBufferSize = 0u;
		for (uint32 i = 0u; (i < numberOfSignals) && (ret); i++) {
			const char8* childName = signalsDatabase.GetChildName(i);
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
			if(ret) {
				uint32 numberOfConsumers=0u;
				GetSignalNumberOfConsumers(i,nextStateNameIn,numberOfConsumers);
				uint32 numberOfProducers=0u;
				GetSignalNumberOfProducers(i,nextStateNameIn,numberOfProducers);

				//read signal
				if (numberOfConsumers>0u) {
					readBufferSize += signalBufferSize;
				}
				//read signal
				else if (numberOfProducers>0u) {
					writeBufferSize += signalBufferSize;
				}
				ret = signalsDatabase.MoveToAncestor(1u);
			}

		}
		if(ret) {
			if(readBufferSize>0u) {
				readMem=new uint8[readBufferSize*numberOfReadBuffers];
			}
			else if(readBufferSize>0u) {
				writeMem=new uint8[writeBufferSize*numberOfWriteBuffers];
			}
		}

	}
	return ret;
}

bool EfficientDriver::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = DataSourceI::SetConfiguredDatabase(data);

	if (ret) {
		//check for buffer size and data type within the
		//signals configuration and allocate the memory
		numberOfSignals = GetNumberOfSignals();
		ret = (numberOfSignals == 1);
	}

	//TODO check all the signals to be scalar and uint32

	return ret;
}

uint64 EfficientDriver::GetReadBufferPointer() {
	//the index changes in the Synchronise fun
	return (uint64) (&readMem[readBufferIndex * readBufferSize]);
}

uint64 EfficientDriver::GetWriteBufferPointer() {
	//the index changes in the Synchronise fun
	return (uint64) (&writeMem[writeBufferIndex * writeBufferSize]);
}


bool EfficientDriver::IsInput(){
	return (uint32)isInput;
}

