/*
 * EfficientReader.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#include "EfficientReader.h"
#include "EfficientDriver.h"
#include "AdvancedErrorManagement.h"

EfficientReader::EfficientReader() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	dataSourceCpuFpga = NULL;
}

EfficientReader::~EfficientReader() {
	// TODO Auto-generated destructor stub
}

bool EfficientReader::Execute() {
	bool ret = true;

	dataSourceCpuFpga->Synchronise();
	uint32 n;
	for (n = 0u; (n < numberOfCopies); n++) {

		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
			uint32 sizeToRead = copyTable[n].copySize;
			//read operation
			/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Received %d",
			 *(uint32*) (copyTable[n].gamPointer));*/
			*(uint64*) (copyTable[n].gamPointer) =
					dataSourceCpuFpga->GetReadBufferPointer();

		}
	}
	return ret;
}

bool EfficientReader::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName,
			gamMemoryAddress);
	if (ret) {
		dataSourceCpuFpga = dynamic_cast<EfficientDriver *>(dataSource);
	}
	return ret;
}

CLASS_REGISTER(EfficientReader, "1.0")
