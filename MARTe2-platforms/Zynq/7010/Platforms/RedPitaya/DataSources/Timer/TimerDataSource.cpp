/**
 * @file TimerDataSource.cpp
 * @brief Source file for class TimerDataSource
 * @date 6 Aug 2016
 * @author andre
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
 * the class TimerDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "TimerDataSource.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TimerDataSource::TimerDataSource() :
		DataSourceI() {
	dataSourceTime = 0u;
}

TimerDataSource::~TimerDataSource() {
}

bool TimerDataSource::Synchronise() {
	//cycles jumped! wait the next
	uint32 factor = (HighResolutionTimer::Counter() - storeCounter)
			/ counterPeriod;
	uint64 period = counterPeriod * (factor + 1);
	//hold on the cycle period
	while ((HighResolutionTimer::Counter() - storeCounter) < period)
		;
	storeCounter = HighResolutionTimer::Counter();
	dataSourceTime += (period * HighResolutionTimer::Period()*1e6);
	return true;
}

bool TimerDataSource::AllocateMemory() {
	return true;
}

uint32 TimerDataSource::GetNumberOfMemoryBuffers() {
	return 1u;
}

bool TimerDataSource::GetSignalMemoryBuffer(const uint32 signalIdx,
		const uint32 bufferIdx, void *&signalAddress) {
	bool ret = (signalIdx == 0u);
	if (ret) {
		signalAddress = reinterpret_cast<void *>(&dataSourceTime);
	}
	return ret;
}

const char8 *TimerDataSource::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if (direction == InputSignals) {
		return "MemoryMapSynchronisedInputBroker";
	}
	return "";
}

bool TimerDataSource::GetInputBrokers(ReferenceContainer &inputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT < MemoryMapSynchronisedInputBroker
			> broker("MemoryMapSynchronisedInputBroker");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = inputBrokers.Insert(broker);
	}
	return ret;
}

bool TimerDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	return false;
}

bool TimerDataSource::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = DataSourceI::SetConfiguredDatabase(data);
	if (ret) {
		ret = (GetNumberOfSignals() == 1);
	}
	if (!ret) {
		REPORT_ERROR(ErrorManagement::InitialisationError,
				"Only one signal can be specified");
	}
	if (ret) {
		ret = (GetSignalType(0)
				== TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
	}
	if (!ret) {
		REPORT_ERROR(ErrorManagement::InitialisationError,
				"The time signal type has to be uint32");
	}
	float32 frequency = 0.;
	if (ret) {
		ret = GetFunctionSignalReadFrequency(InputSignals, 0, 0, frequency);
	}
	if (!ret) {
		REPORT_ERROR(ErrorManagement::InitialisationError,
				"Could not read the time signal frequency");
	}
	if (ret) {
		ret = (frequency > 0.);
	}
	if (!ret) {
		REPORT_ERROR(ErrorManagement::InitialisationError,
				"The frequency has to be > 0");
	}
	if (ret) {
		counterPeriod = (uint64)(
				(HighResolutionTimer::Frequency()) / frequency);

		storeCounter = HighResolutionTimer::Counter();
		dataSourceTime = storeCounter * HighResolutionTimer::Period();
	}

	return ret;
}

bool TimerDataSource::PrepareNextState(
		const MARTe::char8 * const currentStateName,
		const MARTe::char8 * const nextStateName) {
	storeCounter = HighResolutionTimer::Counter();
	dataSourceTime = storeCounter * HighResolutionTimer::Period();
	return true;
}

CLASS_REGISTER(TimerDataSource, "1.0")
