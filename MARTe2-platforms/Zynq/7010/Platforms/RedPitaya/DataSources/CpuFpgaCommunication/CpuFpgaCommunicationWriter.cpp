/*
 * CpuFpgaCommunicationWriter.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#include "CpuFpgaCommunicationWriter.h"
#include "CpuFpgaCommunication.h"
#include "AdvancedErrorManagement.h"

CpuFpgaCommunicationWriter::CpuFpgaCommunicationWriter() :
		MemoryMapOutputBroker() {
	// TODO Auto-generated constructor stub
	bufferAllocated = 0u;
	dataSourceCpuFpga = NULL;
}

CpuFpgaCommunicationWriter::~CpuFpgaCommunicationWriter() {
	// TODO Auto-generated destructor stub
}

bool CpuFpgaCommunicationWriter::Execute() {
	bool ret = true;
	if (bufferAllocated == 1) {
		dataSourceCpuFpga->SyncBuffer();
		ret = MemoryMapOutputBroker::Execute();
		//REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "writer ok");
	} else {
		uint32 n;

		for (n = 0u; (n < numberOfCopies) && (ret); n++) {
			if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
				//TODO usb write op
				//REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,"Executing %d %d", *(uint32*)(copyTable[n].gamPointer),copyTable[n].copySize);
				//dataSourceCpuFpga->SetWriteBufferPointer(*(uint32*)(copyTable[n].gamPointer));
			}
		}
	}
	return ret;
}

bool CpuFpgaCommunicationWriter::Init(const SignalDirection direction,
		DataSourceI &dataSourceIn, const char8 * const functionName,
		void * const gamMemoryAddress) {
	bool ret = MemoryMapOutputBroker::Init(direction, dataSourceIn,
			functionName, gamMemoryAddress);
	if (ret) {
		dataSourceCpuFpga = dynamic_cast<CpuFpgaCommunication *>(dataSource);
		bufferAllocated = dataSourceCpuFpga->IsBufferAllocated();
	}
	return ret;
}

CLASS_REGISTER(CpuFpgaCommunicationWriter, "1.0")

