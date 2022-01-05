/*
 * SinGenerator.cpp
 *
 *  Created on: Feb 16, 2018
 *      Author: codac-dev
 */

#include "SinGenerator.h"
#include "AdvancedErrorManagement.h"

#include <math.h>

SinGenerator::SinGenerator() {
	// TODO Auto-generated constructor stub
	frequency = NULL;
	amplitude=NULL;
	phase=NULL;
	numberOfWaves=0u;
}

SinGenerator::~SinGenerator() {
	// TODO Auto-generated destructor stub
	if (frequency != NULL) {
		delete[] frequency;
	}
	if(amplitude!=NULL) {
		delete[] amplitude;
	}
	if(phase!=NULL) {
		delete[] phase;
	}
}

bool SinGenerator::Initialise(StructuredDataI &data) {
	REPORT_ERROR(ErrorManagement::InitialisationError,
					"Here");
	bool ret = ReferenceContainer::Initialise(data);

	if (ret) {
		//read a list of frequencties
		AnyType at = data.GetType("Frequencies");
		ret = !at.IsVoid();
		if (ret) {
			numberOfWaves = at.GetNumberOfElements(0u);
			frequency = new float[numberOfWaves];
			amplitude = new float[numberOfWaves];
			phase = new float[numberOfWaves];
			Vector<float> frequencyVec(frequency, numberOfWaves);
			ret = data.Read("Frequencies", frequencyVec);
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Frequencies not specified");
			}
		} else {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"Frequencies not specified");
		}
		//a list of amplitudes
		if (ret) {
			Vector<float> amplitudeVec(amplitude, numberOfWaves);
			ret = data.Read("Amplitudes", amplitudeVec);
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Amplitudes not specified");
			}
		}
		//a list of phases
		if (ret) {
			Vector<float> phaseVec(phase, numberOfWaves);
			ret = data.Read("Phases", phaseVec);
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Phases not specified");
			}
		}

	}
	else{
		REPORT_ERROR(ErrorManagement::InitialisationError,
						"WTF");

	}
	return ret;
}

bool SinGenerator::FillMem(uint16* mem, uint32 bufferSize, float factor, float offset) {

	//imagine that the buffer size is a second of time
	MemoryOperationsHelper::Set(mem, '\0', bufferSize);
	uint32 nSamples=bufferSize/sizeof(uint16);
	for (uint32 i = 0u; i < numberOfWaves; i++) {
		for (uint32 t = 0u; t < nSamples; t++) {
			mem[t] += (uint16) ((amplitude[i]
					* sin((2 * M_PI * frequency[i] * t / nSamples) + phase[i]))
					* factor + offset);

		}
	}
	return true;
}

CLASS_REGISTER(SinGenerator, "1.0")

