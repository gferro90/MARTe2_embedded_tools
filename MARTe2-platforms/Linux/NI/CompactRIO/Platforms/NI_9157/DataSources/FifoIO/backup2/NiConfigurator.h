/*
 * NiConfigurator.h
 *
 *  Created on: Feb 19, 2018
 *      Author: codac-dev
 */

#ifndef NICONFIGURATOR_H_
#define NICONFIGURATOR_H_

#include "ReferenceContainer.h"

using namespace MARTe;

class NiConfigurator: public ReferenceContainer {
public:
	CLASS_REGISTER_DECLARATION()

	NiConfigurator();
	virtual ~NiConfigurator();
	virtual bool Initialise(StructuredDataI & data);

};

#endif /* NICONFIGURATOR_H_ */
