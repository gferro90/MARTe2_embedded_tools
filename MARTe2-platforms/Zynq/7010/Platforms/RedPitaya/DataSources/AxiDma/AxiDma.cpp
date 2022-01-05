/*
 * AxiDma.cpp
 *
 *  Created on: Jan 23, 2018
 *      Author: gferro
 */

#include "AxiDma.h"
#include "AdvancedErrorManagement.h"
#include "ObjectRegistryDatabase.h"

AxiDma::AxiDma() {
	// TODO Auto-generated constructor stub

}

AxiDma::~AxiDma() {
	// TODO Auto-generated destructor stub
}

bool AxiDma::Initialise(StructuredDataI &data) {
	bool ret = CpuFpgaCommunication::Initialise(data);
	if (ret) {
		uint32 dmaPtr;
		ret = data.Read("DmaConfigPtr", dmaPtr);
		dmaConfigPtr = (uint32*) (dmaPtr);
	}
	return ret;
}

bool AxiDma::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = CpuFpgaCommunication::SetConfiguredDatabase(data);
	if (ret) {
		StreamString hwCounterPath;
		if (data.Read("HwCounterPath", hwCounterPath)) {
			hwCounter = ObjectRegistryDatabase::Instance()->Find(hwCounterPath.Buffer());
		}
	}
	return ret;
}

bool AxiDma::PrepareNextState(const char8 * const currentStateNameIn,
		const char8 * const nextStateNameIn) {

	bool ret = CpuFpgaCommunication::PrepareNextState(currentStateNameIn,
			nextStateNameIn);
	if (ret) {
		REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "start DMA %d",
				(uint32) dmaConfigPtr);
#if 0
		uint32* dma_ptr = (uint32*) 0x40400030;

		//enable cyclic mode
		dma_ptr[0] |= (1 << 4);

		uint32 invalid;

		//disable interrupts
		dma_ptr[0] &= ~(7 << 12);
		//run dma engine
		dma_ptr[0] |= 0x1;
		//set last descriptor address
		dma_ptr[4] = (uint32) (&invalid);
		dma_ptr[5] = 0;
#endif
		uint32 invalid;

		//enable cyclic mode
		dmaConfigPtr[0] |= (1 << 4);

		//disable interrupts
		dmaConfigPtr[0] &= ~(7 << 12);
		//run dma engine
		dmaConfigPtr[0] |= 0x1;
		//start hw counter
		if(hwCounter.IsValid()){
			hwCounter->Restart();
		}
		//set last descriptor address
		dmaConfigPtr[4] = (uint32)(&invalid);
		dmaConfigPtr[5] = 0;
	}
	return ret;

}

CLASS_REGISTER(AxiDma, "1.0")
