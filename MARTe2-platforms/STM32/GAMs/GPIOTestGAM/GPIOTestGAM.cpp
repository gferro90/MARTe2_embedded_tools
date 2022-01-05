/**
 * @file GPIOTestGAM.cpp
 * @brief Source file for class GPIOTestGAM
 * @date 6 Aug 2016
 * @author andre
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class GPIOTestGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GPIOTestGAM.h"

#include "AdvancedErrorManagement.h"
#include "string.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
static uint8 status=0;

GPIOTestGAM::GPIOTestGAM() :
		GAM() {
}


bool GPIOTestGAM::Execute() {
	uint32 mask=(1<<12)|(1<<14)|(1<<13);

	*(uint32*) GetOutputSignalsMemory() = mask*(1-status)|((mask)<<(16*status));//*(uint32*) GetInputSignalsMemory();

	status=(status+1)%2;

	return true;
}
CLASS_REGISTER(GPIOTestGAM, "1.0")
