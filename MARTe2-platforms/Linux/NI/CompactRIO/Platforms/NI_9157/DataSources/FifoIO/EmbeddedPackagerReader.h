/*
 * EmbeddedPackagerReader.h
 *
 *  Created on: Feb 22, 2018
 *      Author: codac-dev
 */

#ifndef EMBEDDEDPACKAGERREADER_H_
#define EMBEDDEDPACKAGERREADER_H_

#include "SimpleReader.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class CircularBufferAcqDriver;

class EmbeddedPackagerReader: public SimpleReader {
public:
	CLASS_REGISTER_DECLARATION()
	EmbeddedPackagerReader();
	virtual ~EmbeddedPackagerReader();
	virtual bool Execute();

};





#endif /* EMBEDDEDPACKAGERREADER_H_ */
