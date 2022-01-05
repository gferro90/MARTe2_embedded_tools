/*
 * EfficientWriter.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#include "EfficientWriter.h"
#include "EfficientDriver.h"
#include "AdvancedErrorManagement.h"

EfficientWriter::EfficientWriter() :
		MemoryMapOutputBroker() {
	// TODO Auto-generated constructor stub
	dataSourceCpuFpga = NULL;
}

EfficientWriter::~EfficientWriter() {
	// TODO Auto-generated destructor stub
}

bool EfficientWriter::Execute() {
	bool ret = true;
	uint32 n;

	for (n = 0u; (n < numberOfCopies) && (ret); n++) {
		if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {

			//TODO usb write op
			//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,"Executing %d %d", *(uint32*)(copyTable[n].gamPointer),copyTable[n].copySize);
			dataSourceCpuFpga->SetWriteBufferPointer(*(uint64*)(copyTable[n].gamPointer));
		}
	}
	dataSourceCpuFpga->Synchronise();

	return ret;
}

bool EfficientWriter::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapOutputBroker::Init(direction, dataSourceIn,
			functionName, gamMemoryAddress);
	if (ret) {
		dataSourceCpuFpga = dynamic_cast<EfficientDriver *>(dataSource);
	}
	return ret;
}

CLASS_REGISTER(EfficientWriter, "1.0")

