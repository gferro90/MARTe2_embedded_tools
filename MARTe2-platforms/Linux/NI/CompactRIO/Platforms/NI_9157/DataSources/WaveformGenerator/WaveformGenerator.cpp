/*
 * WaveformGenerator.cpp
 *
 *  Created on: Feb 15, 2018
 *      Author: codac-dev
 */

#include "WaveformGenerator.h"
#include "AdvancedErrorManagement.h"
#include "FunctionGenerator.h"

WaveformGenerator::WaveformGenerator() {
	// TODO Auto-generated constructor stub
	factor=1.;
	offset=0.;
}

WaveformGenerator::~WaveformGenerator() {
	// TODO Auto-generated destructor stub
}


bool WaveformGenerator::Initialise(StructuredDataI &data){
	bool ret=MemoryProvider::Initialise(data);
	if(ret){
		if(!data.Read("Factor", factor)){
			factor=1.0;
		}
		if(!data.Read("Offset", offset)){
			offset=0.0;
		}
	}
	return ret;
}

bool WaveformGenerator::SetConfiguredDatabase(StructuredDataI & data) {
	bool ret = MemoryProvider::SetConfiguredDatabase(data);

	if (ret) {
		for (uint32 n = 0u; (n < nOfSignals) && (ret); n++) {
			const char8* childName = signalsDatabase.GetChildName(n);
			ret = signalsDatabase.MoveRelative(childName);
			if (ret) {
				StreamString functionGeneratorName;
				signalsDatabase.Read("FunctionGeneratorName",
						functionGeneratorName);
				uint32 numberOfFunctions = Size();
				REPORT_ERROR_PARAMETERS(ErrorManagement::Information,
						"FunctionGeneratorName[%d]=%s", n,
						functionGeneratorName.Buffer());
				for (uint32 j = 0u; j < numberOfFunctions; j++) {
					ReferenceT<FunctionGenerator> functionGen = Get(j);
					if (functionGen.IsValid()) {

						if (functionGeneratorName == functionGen->GetName())
						functionGen->FillMem((uint16 *) bufferCopy[n],
								bufferSize[n], factor, offset);
						break;
					}
				}
				if (ret) {
					ret = signalsDatabase.MoveToAncestor(1u);
				}
			}
		}
	}
	return ret;
}
CLASS_REGISTER(WaveformGenerator, "1.0")
