/*
 * GPIOReader.cpp
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#include "GPIOReader.h"
#include "AdvancedErrorManagement.h"
#include "GPIO.h"


GPIOReader::GPIOReader() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	gpioHandle = NULL;
}

GPIOReader::~GPIOReader() {
	// TODO Auto-generated destructor stub
}

bool GPIOReader::Execute() {
	uint32 n;
	bool ret = true;
	for (n = 0u; (n < numberOfCopies) && (ret); n++) {
		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
			uint32 reg = (gpioHandle->IDR);
			MemoryOperationsHelper::Copy(copyTable[n].gamPointer, &reg,
					copyTable[n].copySize);
		}
	}
	return ret;
}

bool GPIOReader::Init(const SignalDirection direction,
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

CLASS_REGISTER(GPIOReader, "1.0")
