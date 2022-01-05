/*
 * EmbeddedCopyGAM.h
 *
 *  Created on: Jan 25, 2018
 *      Author: gferro
 */

#ifndef EMBEDDEDCOPYGAM_H_
#define EMBEDDEDCOPYGAM_H_

#include "GAM.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
class EmbeddedCopyGAM: public GAM {
public:
	CLASS_REGISTER_DECLARATION()

	EmbeddedCopyGAM();
	virtual ~EmbeddedCopyGAM();

	virtual bool Execute();
	virtual bool Setup();
private:
    int32 *decFactor;
    uint32 *maxBuffSize;
    uint32 *inputOffset;
    uint32 *outputOffset;
    uint8 *dataSize;

};

#endif /* EMBEDDEDCOPYGAM_H_ */
