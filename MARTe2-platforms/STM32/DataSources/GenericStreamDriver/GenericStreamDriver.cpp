/**
 * @file GenericStreamDriver.cpp
 * @brief Source file for class GenericStreamDriver
 * @date Jun 16, 2017
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class GenericStreamDriver (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GenericStreamDriver.h"

#include "AdvancedErrorManagement.h"
#include "GenericStreamReader.h"
#include "GenericStreamWriter.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters
GenericStreamDriver::GenericStreamDriver() :
		GAMDataSource() {

	allocateBuffer = 0u;
	numberOfSignals = 0u;
	numberOfFunctions = 0u;
	opCounter = 0u;

}

GenericStreamDriver::~GenericStreamDriver() {

}

bool GenericStreamDriver::Initialise(StructuredDataI &data) {
	bool ret = GAMDataSource::Initialise(data);
	if (ret) {
		if (!data.Read("AllocateBuffer", allocateBuffer)) {
			allocateBuffer = 0u;
		}

	}

	return ret;
}

bool GenericStreamDriver::Synchronise() {
	return true;
}

bool GenericStreamDriver::AllocateMemory() {
	if (allocateBuffer) {
		return GAMDataSource::AllocateMemory();
	}
	return true;
}

uint32 GenericStreamDriver::GetNumberOfMemoryBuffers() {
	return (allocateBuffer) ? (GAMDataSource::GetNumberOfMemoryBuffers()) : 1u;
}

bool GenericStreamDriver::GetSignalMemoryBuffer(const uint32 signalIdx,
		const uint32 bufferIdx, void *&signalAddress) {
	if (allocateBuffer) {
		GAMDataSource::GetSignalMemoryBuffer(signalIdx, bufferIdx,
				signalAddress);
	} else {
		signalAddress = NULL;
	}
	return true;
}

bool GenericStreamDriver::PrepareNextState(
		const MARTe::char8 * const currentStateNameIn,
		const MARTe::char8 * const nextStateNameIn) {
	/*bool ret = true;
	//read the total number of consumers and producers for the nextStateName
	if (allocateBuffer) {
		ret = GAMDataSource::PrepareNextState(currentStateNameIn,
				nextStateNameIn);
	}*/
	numberOfSignals = GetNumberOfSignals();
	currentStateName = nextStateNameIn;
	for (uint32 i = 0u; i < numberOfSignals; i++) {
		numberOfFunctions = GetNumberOfFunctions();
	}

	return true;
}

bool GenericStreamDriver::SetConfiguredDatabase(StructuredDataI & data) {
	return DataSourceI::SetConfiguredDatabase(data);
}

uint8 GenericStreamDriver::IsBufferAllocated() const {
	return allocateBuffer;
}

void GenericStreamDriver::SyncBuffer() {

	//this function should be called from all the gams reading this ds signals
	//so after numberOfFunctions reads, a cycle has passed
	//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "opCounter %d %d", opCounter, numberOfFunctions);

	if (opCounter % numberOfFunctions == 0) {
		for (uint32 i = 0u; i < numberOfSignals; i++) {
			/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "nSignals %d ",
					numberOfSignals);*/
			uint32 signalNumberOfConsumers = 0u;
			uint32 signalNumberOfProducers = 0u;
			GetSignalNumberOfConsumers(i, currentStateName.Buffer(),
					signalNumberOfConsumers);
			GetSignalNumberOfProducers(i, currentStateName.Buffer(),
					signalNumberOfProducers);
			/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
					"prd %d, cons %d %s %d", signalNumberOfProducers,
					signalNumberOfConsumers, currentStateName.Buffer(), i);
			*/
			void *bufferPtr = NULL;
			// if this is a read signal
			if (signalNumberOfConsumers > 0u) {
				if (GAMDataSource::GetSignalMemoryBuffer(i, 0u, bufferPtr)) {
					if (bufferPtr != NULL) {
						uint32 sizeToRead = 0u;
						if (GetSignalByteSize(i, sizeToRead)) {
							Read(bufferPtr, sizeToRead);
							//int x = *(uint32*) bufferPtr;
							//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "received %d %d", x, sizeToRead);
						}
					}
				}
			} else if (signalNumberOfProducers > 0u) {
				if (GAMDataSource::GetSignalMemoryBuffer(i, 0u, bufferPtr)) {
					if (bufferPtr != NULL) {
						uint32 sizeToWrite = 0u;
						if (GetSignalByteSize(i, sizeToWrite)) {
							Write(bufferPtr, sizeToWrite);
							//int x = *(uint32*) bufferPtr;
							//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "writed %d %d", x, sizeToWrite);
						}
					}
				}
			}

		}
		opCounter = 0u;
	}
	opCounter++;

}

const char8 *GenericStreamDriver::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if (direction == InputSignals) {
		return "GenericStreamReader";
	}
	if (direction == OutputSignals) {
		return "GenericStreamWriter";
	}
	return "";
}

bool GenericStreamDriver::GetInputBrokers(ReferenceContainer &inputBrokers,
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

bool GenericStreamDriver::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<GenericStreamWriter> broker("GenericStreamWriter");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = outputBrokers.Insert(broker);
	}
	return ret;
}

void GenericStreamDriver::TerminateRead() {

}

void GenericStreamDriver::TerminateWrite() {

}

