/*
 * GPIOWriter.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "GPIOWriter.h"
#include "AdvancedErrorManagement.h"
#include "GPIO.h"

GPIOWriter::GPIOWriter() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	gpioHandle = NULL;
}

GPIOWriter::~GPIOWriter() {
	// TODO Auto-generated destructor stub
}

bool GPIOWriter::Execute() {
	//REPORT_ERROR(ErrorManagement::Warning, "Writing");
	uint32 n;
	bool ret = true;
	for (n = 0u; (n < numberOfCopies) && (ret); n++) {
		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
			gpioHandle->BSRR = *(uint32*) copyTable[n].gamPointer;
			//HAL_GPIO_WritePin( gpioHandle, (*(uint16*)copyTable[n].gamPointer), GPIO_PIN_SET);
		}
	}
	return ret;
}

bool GPIOWriter::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName,
			gamMemoryAddress);
	GPIO *gpioDS = dynamic_cast<GPIO*>(dataSource);
	if (gpioDS != NULL) {
		gpioHandle = gpioDS->GetHwHandle();
	} else {
		REPORT_ERROR(ErrorManagement::FatalError,
				"Dynamic cast from DataSourceI to GPIO failed");
	}
	return ret;
}

CLASS_REGISTER(GPIOWriter, "1.0")
