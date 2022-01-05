/*
 * GPIO.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "GPIO.h"
#include "AdvancedErrorManagement.h"
#include "GPIOReader.h"
#include "GPIOWriter.h"
#include "HandleDatabase.h"

//TODO
//the standard USBCommunicationReader is not sync and it is blocking
//add the sync and the non-blocking one and return them depending on configuration parameters



GPIO::GPIO() :
		DataSourceI() {
	//Initialisation already done by the tool !!!
	gpioHandlePtr = NULL;
}

GPIO::~GPIO() {
}

bool GPIO::Initialise(StructuredDataI &data) {
	bool ret = DataSourceI::Initialise(data);
	if (ret) {
		StreamString gpioId;
		ret = data.Read("GpioIdentifier", gpioId);
		if (ret) {
			gpioHandlePtr = (GPIO_TypeDef *) GetHandle(gpioId.Buffer());
			ret = (gpioHandlePtr != NULL);
		} else {
			REPORT_ERROR(ErrorManagement::Warning, "No GPIO specified");
		}
	}

	return ret;
}

bool GPIO::Synchronise() {
	return true;
}

bool GPIO::AllocateMemory() {
	return true;
}

uint32 GPIO::GetNumberOfMemoryBuffers() {
	return 1u;
}

bool GPIO::SetConfiguredDatabase(MARTe::StructuredDataI & data) {
	bool ret = DataSourceI::SetConfiguredDatabase(data);

	if (ret) {
		uint32 numberOfSignals = GetNumberOfSignals();

		for (uint32 i = 0u; i < numberOfSignals && ret; i++) {

			ret = (GetSignalType(i)
					== TypeDescriptor::GetTypeDescriptorFromTypeName("uint32"));
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"The gpio signal type has to be uint32");
			}
			if (ret) {
				uint8 nDims = 0u;
				ret = (GetSignalNumberOfDimensions(i, nDims));
				if (ret) {
					ret = nDims == 0u;
					if (!ret) {
						REPORT_ERROR(ErrorManagement::InitialisationError,
								"The gpio signal is a scalar");
					}
				}
			}
		}
	}

	return ret;
}

bool GPIO::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx,
		void *&signalAddress) {
	signalAddress = NULL;
	return true;
}

const char8 *GPIO::GetBrokerName(StructuredDataI &data,
		const SignalDirection direction) {
	if (direction == InputSignals) {
		return "GPIOReader";
	}
	if (direction == OutputSignals) {
		return "GPIOWriter";
	}
	return "";
}

bool GPIO::GetInputBrokers(ReferenceContainer &inputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<GPIOReader> broker("GPIOReader");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = inputBrokers.Insert(broker);
	}
	return ret;
}

bool GPIO::GetOutputBrokers(ReferenceContainer &outputBrokers,
		const char8* const functionName, void * const gamMemPtr) {
	ReferenceT<GPIOWriter> broker("GPIOWriter");
	bool ret = broker.IsValid();
	if (ret) {
		ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
	}
	if (ret) {
		ret = outputBrokers.Insert(broker);
	}
	return ret;
}

bool GPIO::PrepareNextState(const MARTe::char8 * const currentStateName,
		const MARTe::char8 * const nextStateName) {
	return true;
}

GPIO_TypeDef * GPIO::GetHwHandle() {
	//todo in the main map into an array the handles of the defined uarts
	//define this function
	return gpioHandlePtr;
}

CLASS_REGISTER(GPIO, "1.0")
