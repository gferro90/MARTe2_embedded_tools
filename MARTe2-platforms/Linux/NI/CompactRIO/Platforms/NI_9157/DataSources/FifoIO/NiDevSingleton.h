/*
 * NiDevSingleton.h
 *
 *  Created on: Feb 19, 2018
 *      Author: codac-dev
 */

#ifndef NIDEVSINGLETON_H_
#define NIDEVSINGLETON_H_

#include "ReferenceContainer.h"

using namespace MARTe;

class NiDevSingleton: public ReferenceContainer {
public:
	NiDevSingleton* Instance();
	virtual ~NiDevSingleton();
	bool IsInitialised() const;


private:



	NiDevSingleton();
	NiDevSingleton *me;
	NiFpga_Session session;
	NiFpga_Status status;

};






#endif /* NIDEVSINGLETON_H_ */
