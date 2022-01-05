/*
 * IpBlockConfig.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef IPBLOCKCONFIG_H_
#define IPBLOCKCONFIG_H_

#include "ReferenceContainer.h"

using namespace MARTe;

class IpBlockConfig: public ReferenceContainer {
public:
	CLASS_REGISTER_DECLARATION()

	IpBlockConfig();
	virtual ~IpBlockConfig();

	virtual bool Initialise(StructuredDataI &data);

};

#endif /* IPBLOCKCONFIG_H_ */
