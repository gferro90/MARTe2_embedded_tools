/*
 * EmbeddedCopyGAM.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: gferro
 */

#include "EmbeddedCopyGAM.h"
#include "AdvancedErrorManagement.h"

EmbeddedCopyGAM::EmbeddedCopyGAM() :
		GAM() {
	decFactor = NULL;
	maxBuffSize = NULL;
	inputOffset = NULL;
	outputOffset = NULL;
}

EmbeddedCopyGAM::~EmbeddedCopyGAM() {
	// TODO Auto-generated destructor stub
	if (decFactor != NULL) {
		delete[] decFactor;
	}
	if (maxBuffSize != NULL) {
		delete[] maxBuffSize;
	}
	if (inputOffset != NULL) {
		delete[] inputOffset;
	}
	if (outputOffset != NULL) {
		delete[] outputOffset;
	}
	decFactor = NULL;
	maxBuffSize = NULL;
	inputOffset = NULL;
	outputOffset = NULL;
}

bool EmbeddedCopyGAM::Execute() {
	for (uint32 i = 0; i < numberOfInputSignals; i++) {
		/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
					"dmaPointer=%x", (uint32)(*(char8 **) (GetInputSignalMemory(0))));
*/
		char8* inputSignalBegPtr = (*(char8 **) (GetInputSignalMemory(i)))
				+ inputOffset[i];
		char8* outputSignalBegPtr = ((char8 *) (GetOutputSignalMemory(i)))
				+ outputOffset[i];
		uint32 cntSize = 0;
		uint32 cnt = 0;

		//inputSize > outputSize
		if (decFactor[i] < 0) {

			while (cntSize < maxBuffSize[i]) {
				MemoryOperationsHelper::Copy(outputSignalBegPtr + cntSize,
						inputSignalBegPtr + cnt, dataSize[i]);
				cntSize += (uint32)(-decFactor[i]);
				cnt += dataSize[i];
			}
		} else {
			while (cntSize < maxBuffSize[i]) {
				/*REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
				 "%d from %x to %x",
				 *(int32*)(inputSignalBegPtr + cntSize),
				 (uint32)(inputSignalBegPtr + cntSize),
				 (uint32)(outputSignalBegPtr + cnt));*/

				MemoryOperationsHelper::Copy(outputSignalBegPtr + cnt,
						inputSignalBegPtr + cntSize, dataSize[i]);
				cntSize += decFactor[i];
				cnt += dataSize[i];
			}
		}

	}
	return true;

}

bool EmbeddedCopyGAM::Setup() {
	bool ret = (numberOfInputSignals == numberOfOutputSignals);
	//TODO check the buffer size with the output number of elements
	if (!ret) {
		REPORT_ERROR(ErrorManagement::InitialisationError,
				"The number of input and output signals must be equal!");
	}
	if (ret) {
		REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "NInputSignals=%d",
				numberOfInputSignals);

		REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "NOutputSignals=%d",
				numberOfOutputSignals);

		//move to each input signal and read BufferSize
		ret = signalsDatabase.MoveRelative("InputSignals");
		if (!ret) {
			REPORT_ERROR(ErrorManagement::Warning,
					"Failed move to InputSignals");

		}
		decFactor = new int32[numberOfInputSignals];
		inputOffset = new uint32[numberOfInputSignals];
		outputOffset = new uint32[numberOfOutputSignals];
		maxBuffSize = new uint32[numberOfOutputSignals];
		dataSize = new uint8[numberOfOutputSignals];

		if (ret) {
			for (uint32 i = 0; (i < numberOfInputSignals) && (ret); i++) {
				const char8* childName=signalsDatabase.GetChildName(i);
				ret = signalsDatabase.MoveRelative(childName);
				if (ret) {
					ret = signalsDatabase.Read("BufferSize", decFactor[i]);
					if (ret) {
						REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
													"Read BufferSize=%d for signal %s", decFactor[i],childName);

						maxBuffSize[i] = decFactor[i];
						if (!signalsDatabase.Read("BufferOffset",
								inputOffset[i])) {
							inputOffset[i] = 0;
						}
					} else {
						REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
								"Failed read BufferSize for signal %s", childName);
					}
					ret = signalsDatabase.MoveToAncestor(1u);
				}
			}
			ret = signalsDatabase.MoveToAncestor(1u);
		}
	}
	if (ret) {
		//move to each input signal and read BufferSize
		ret = signalsDatabase.MoveRelative("OutputSignals");
		if (!ret) {
			REPORT_ERROR(ErrorManagement::Warning,
					"Failed move to OutputSignals");
		}
		if (ret) {
			for (uint32 i = 0; (i < numberOfOutputSignals) && (ret); i++) {
				uint32 byteSize;
				GetSignalByteSize(OutputSignals, i, byteSize);
				dataSize[i] = byteSize;
				uint32 nElements = 0;
				GetSignalNumberOfElements(OutputSignals, i, nElements);
				dataSize[i] /= nElements;
				const char8* childName = signalsDatabase.GetChildName(i);
				ret = signalsDatabase.MoveRelative(childName);
				if (ret) {
					int32 tmp;
					ret = signalsDatabase.Read("BufferSize", tmp);
					if (ret) {
						if (tmp > maxBuffSize[i]) {
							maxBuffSize[i] = (uint32)tmp;
						}
						if (tmp <= decFactor[i]) {
							decFactor[i] /= tmp;
						} else {
							decFactor[i] /= (-tmp);
						}
						decFactor[i]*=dataSize[i];
						if (!signalsDatabase.Read("BufferOffset",
								outputOffset[i])) {
							outputOffset[i] = 0;
						}

					} else {
						//TODO error
					}
					ret = signalsDatabase.MoveToAncestor(1u);
				}
				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
						"signal %s BufferSize=%d", childName, maxBuffSize[i]);
				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
						"signal %s InputOffset=%d", childName, inputOffset[i]);
				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
						"signal %s OutputOffset=%d", childName,
						outputOffset[i]);
				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
						"signal %s DataSize=%d", childName, dataSize[i]);
				REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
						"signal %s DecFactor=%d", childName, decFactor[i]);


			}
			ret = signalsDatabase.MoveToAncestor(1u);
		}
	}

	return ret;
}

CLASS_REGISTER(EmbeddedCopyGAM, "1.0")

