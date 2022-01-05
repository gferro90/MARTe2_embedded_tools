/*
 * FifoAcquisition.h
 *
 *  Created on: Feb 14, 2018
 *      Author: codac-dev
 */

#ifndef FIFOACQUISITION_H_
#define FIFOACQUISITION_H_

#include "EfficientDriver.h"
#include "SearchLabviewGen.h"

using namespace MARTe;


class FifoAcquisition: public EfficientDriver {
public:
	CLASS_REGISTER_DECLARATION()
FifoAcquisition();
	virtual ~FifoAcquisition();
	virtual bool Initialise(StructuredDataI & data);

	bool PrepareNextState(const char8 * const currentStateName,
			const char8 * const nextStateName);
    virtual bool Synchronise();

protected:
    NiFpga_Session session;
    uint32 fifoDev;
    int32 store;
};

#endif /* FIFOACQUISITION_H_ */
