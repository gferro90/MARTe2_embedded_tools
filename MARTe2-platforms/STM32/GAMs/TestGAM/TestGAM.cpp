/**
 * @file TestGAM.cpp
 * @brief Source file for class TestGAM
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
 * the class TestGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "TestGAM.h"

#include "AdvancedErrorManagement.h"
#include "string.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
static uint32 values[] = {15, 14, 10 ,14, 15, 16, 20, 16};
static uint32 k=0;

TestGAM::TestGAM() :
		GAM() {
	totalSignalsByteSize = 0u;
}

bool TestGAM::SetConfiguredDatabase(MARTe::StructuredDataI & data) {

	bool ret = GAM::SetConfiguredDatabase(data);
	return ret;
}

bool TestGAM::Execute() {
	*(uint32*) GetOutputSignalsMemory() = values[k];//*(uint32*) GetInputSignalsMemory();

	((uint32*) GetOutputSignalsMemory())[1] = values[k];

	if (*(uint32*) GetInputSignalsMemory() % 4000000 == 0) {
		k++;
		if (k > 7) {
			k = 0;
		}
	}
	return true;
}
CLASS_REGISTER(TestGAM, "1.0")
