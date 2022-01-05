/*
 * EmbeddedPackagerSyncReader.h
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

class EmbeddedPackagerSyncReader: public SimpleReader {
public:
	CLASS_REGISTER_DECLARATION()
	EmbeddedPackagerSyncReader();
	virtual ~EmbeddedPackagerSyncReader();
	virtual bool Execute();

};





#endif /* EMBEDDEDPACKAGERREADER_H_ */
