/*
 * IpBlockConfig.cpp
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#include "IpBlockConfig.h"

IpBlockConfig::IpBlockConfig() {
	// TODO Auto-generated constructor stub

}

IpBlockConfig::~IpBlockConfig() {
	// TODO Auto-generated destructor stub
}

bool IpBlockConfig::Initialise(StructuredDataI &data) {
	bool ret = ReferenceContainer::Initialise(data);

	if (ret) {
		AnyType at = data.GetType("IPconfig");
		ret = !at.IsVoid();
		if (ret) {

			uint32 numberOfElements = at.GetNumberOfElements(0u);
			Vector < uint32 > config(numberOfElements);
			ret = data.Read("IPconfig", config);
			if (ret) {
				volatile uint32* memPtr = (volatile uint32*) config[0];
				for (uint32 i = 1u; i < numberOfElements; i++) {
					//configure the IP block
					memPtr[i - 1] = config[i];
				}

			} else {
				//TODO error
			}

		} else {
			//TODO error
		}
	}
	return ret;
}

CLASS_REGISTER(IpBlockConfig, "1.0")
