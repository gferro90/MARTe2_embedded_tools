/*
 * CpuFpgaCommunicationReader.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#include "CpuFpgaCommunicationReader.h"
#include "CpuFpgaCommunication.h"
#include "AdvancedErrorManagement.h"

CpuFpgaCommunicationReader::CpuFpgaCommunicationReader() :
		MemoryMapInputBroker() {
	// TODO Auto-generated constructor stub
	bufferAllocated = 0u;
	dataSourceCpuFpga = NULL;
}

CpuFpgaCommunicationReader::~CpuFpgaCommunicationReader() {
	// TODO Auto-generated destructor stub
}

bool CpuFpgaCommunicationReader::Execute() {
	bool ret = true;

	if (bufferAllocated == 1) {
		dataSourceCpuFpga->SyncBuffer();
		ret = MemoryMapInputBroker::Execute();
		//REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "reader ok");
	} else {
		dataSourceCpuFpga->Synchronise();
		uint32 n;
		for (n = 0u; (n < numberOfCopies); n++) {

			if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
				uint32 sizeToRead = copyTable[n].copySize;
				//read operation
				/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Received %d",
						*(uint32*) (copyTable[n].gamPointer));*/
				*(uint32*) (copyTable[n].gamPointer) =
						dataSourceCpuFpga->GetReadBufferPointer();

			}
		}
	}
	return ret;
}

bool CpuFpgaCommunicationReader::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName,
			gamMemoryAddress);
	if (ret) {
		dataSourceCpuFpga = dynamic_cast<CpuFpgaCommunication *>(dataSource);
		bufferAllocated = dataSourceCpuFpga->IsBufferAllocated();
	}
	return ret;
}

CLASS_REGISTER(CpuFpgaCommunicationReader, "1.0")
