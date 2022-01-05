/*
 * AxiDma.h
 *
 *  Created on: Jan 23, 2018
 *      Author: gferro
 */

#ifndef AXIDMA_H_
#define AXIDMA_H_

#include "CpuFpgaCommunication.h"
#include "HwCounter.h"

using namespace MARTe;

class AxiDma: public CpuFpgaCommunication {
public:
	CLASS_REGISTER_DECLARATION()

	AxiDma();
	virtual ~AxiDma();

	virtual bool Initialise(StructuredDataI &data);

    virtual bool SetConfiguredDatabase(StructuredDataI & data);

	virtual bool PrepareNextState(const char8 * const currentStateNameIn,
			const char8 * const nextStateNameIn);
private:
	uint32 *dmaConfigPtr;
	ReferenceT<HwCounter> hwCounter;
};

#endif /* AXIDMA_H_ */
