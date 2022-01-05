/*
 * FifoIO.cpp
 *
 *  Created on: Feb 14, 2018
 *      Author: codac-dev
 */

#include "FifoIO.h"
#include "AdvancedErrorManagement.h"
#include "ObjectRegistryDatabase.h"

FifoIO::FifoIO() :
		EfficientDriver() {
	// TODO Auto-generated constructor stub

	fifoDev = 0;
	sampleByteSize = 0u;
	cycleTimeUs = 0u;
	inputMaxVal = 0;
	inputResolution = 0;
	outputMaxVal = 0;
	outputResolution = 0;
	inputTriggerVar = 0;
	outputTriggerVar = 0;
	isTrigger = 0;
	niDevice = NULL;
	timeout = 0u;
	runNi = 0u;
}

FifoIO::~FifoIO() {
	if (niDevice != NULL) {
		delete niDevice;
	}
}

bool FifoIO::Initialise(StructuredDataI & data) {
	bool ret = EfficientDriver::Initialise(data);
	NiFpga_Status status;

	if (ret) {
		StreamString fifoName;
		ret = data.Read("FifoName", fifoName);
		if (ret) {
			fifoDev = SearchLabviewGen(fifoName.Buffer(), ret);
			if (!ret) {
				REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
						"Fifo name %s not found", fifoName.Buffer());
			}
		} else {
			REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
					"Cannot find FifoName=%s", fifoName.Buffer());
		}
	}
	if (ret) {
		StreamString devicePath;
		ret = data.Read("NiRioDevicePath", devicePath);
		if (ret) {
			niDeviceBoard = ObjectRegistryDatabase::Instance()->Find(
					devicePath.Buffer());
			ret = niDeviceBoard.IsValid();
			if (!ret) {
				REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
						"Cannot find the the NI-Dev at path %s",
						devicePath.Buffer());
			}
		} else {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"Please specify the NI-Dev path (NiRioDevicePath)");
		}
		if (ret) {
			StreamString deviceOperatorName;
			ret = data.Read("NiRioDeviceOperator", deviceOperatorName);
			if (ret) {
				CreateNiDevice *creator = GetNiDevice(
						deviceOperatorName.Buffer());
				ret = (creator != NULL);
				if (ret) {
					niDevice = creator->Create(niDeviceBoard);
					ret = (niDevice != NULL);
					if (ret) {
						sampleByteSize = niDevice->GetByteSize();
					} else {
						REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
								"Failed creating the NiDevice operator %s",
								deviceOperatorName.Buffer());

					}
				} else {
					REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
							"Cannot find the CreateNiDevice %s",
							deviceOperatorName.Buffer());
				}
			} else {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Please specify the NI-Dev operator (NiRioDeviceOperator)");

			}
		}
	} else {
		REPORT_ERROR(ErrorManagement::InitialisationError,
				"Please specify the FIFO name (FifoName)");

	}
	if (ret) {
		if (!data.Read("Timeout", timeout)) {
			timeout = 0xFFFFFFFF;
			REPORT_ERROR_PARAMETERS(ErrorManagement::Warning,
					"Timeout not specified: assumed default %d", timeout);
		}

	}
	if (ret) {
		if (!data.Read("RunNi", runNi)) {
			runNi = 1;
		}
	}

	return ret;
}

bool FifoIO::PrepareNextState(const char8 * const currentStateName,
		const char8 * const nextStateName) {
	bool ret = EfficientDriver::PrepareNextState(currentStateName,
			nextStateName);
	NiFpga_Status status;
	if (ret) {
		if (IsInput()) {
			uint32 hostFifoSize = (readBufferSize * numberOfReadBuffers)
					/ sampleByteSize;
			uint32 oldSize = 0;
			status = niDeviceBoard->NiConfigureFifo(fifoDev, hostFifoSize,
					oldSize);
			ret = (status == 0);
			if (!ret) {
				REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
						"Failed to configure host FIFO size to %d, status=%d %d",
						hostFifoSize, status, numberOfReadBuffers);
			}
		} else {
			uint32 hostFifoSize = (writeBufferSize * numberOfWriteBuffers)
					/ sampleByteSize;
			uint32 oldSize = 0;
			status = niDeviceBoard->NiConfigureFifo(fifoDev, hostFifoSize,
					oldSize);
			ret = (status == 0);
			if (!ret) {
				REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
						"Failed to configure host FIFO size to %d, status=%d %d",
						hostFifoSize, status, numberOfWriteBuffers);
			}
		}
	}
	if (ret) {
		if (runNi == 1) {
			REPORT_ERROR(ErrorManagement::Information, "Running...");
			status = niDeviceBoard->Run();
			ret = (status == 0);
			if (!ret) {
				REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
						"Failed to Run the application, status=%d", status);
			}
		}
	}
	if (ret) {
		readBufferIndex = 1;
		writeBufferIndex = 1;
	}
	return ret;
}

bool FifoIO::Synchronise() {
	uint32 fifoRemaining = 0u;
	NiFpga_Status status = 0;
	if (IsInput()) {
		readBufferIndex++;
		readBufferIndex %= numberOfReadBuffers;
		REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Reading %d ",
				readBufferSize / sampleByteSize);
		//todo configurable timeout
		status = niDevice->NiReadFifo(fifoDev,
				(uint8*) (&readMem[readBufferIndex * readBufferSize]),
				readBufferSize / sampleByteSize, timeout, fifoRemaining);

		/*
		 niDeviceBoard->NiWrite((uint32)0x812A, (uint8)3);

		 while(1){
		 uint32 packCnt;
		 NiFpga_Session session=niDeviceBoard->GetSession();
		 NiFpga_ReadU32(session, 0x810C, &packCnt);
		 //REPORT_ERROR_PARAMETERS(ErrorManagement::Information,"packet %d ", packCnt);

		 }
		 */
		uint32 current =
				((uint16*) (&readMem[readBufferIndex * readBufferSize]))[readBufferSize
						/ sampleByteSize - 1];
		REPORT_ERROR_PARAMETERS(ErrorManagement::Information,
				"Read %d remaining=%d status=%d", current, fifoRemaining,
				status);
		/*REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Read %d",
		 current);*/
		//store = current;
	} else {

		writeBufferIndex++;
		writeBufferIndex %= numberOfWriteBuffers;

		//todo configurable timeout
		status = niDevice->NiWriteFifo(fifoDev,
				(uint8*) (&writeMem[writeBufferIndex * writeBufferSize]),
				writeBufferSize / sampleByteSize, timeout, fifoRemaining);

		uint16 current = *(uint16*) (&writeMem[writeBufferIndex
				* writeBufferSize]);
		/*REPORT_ERROR_PARAMETERS("Read %d remaining=%d status=%d gap=%d", current,
		 fifoRemaining, status, (current - store));*/
		/*REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Write %d %d %x %d",
		 current, writeBufferIndex * writeBufferSize, (uint64 )writeMem, sampleByteSize);*/
		//store = current;
	}
	return (status == 0);
}

CLASS_REGISTER(FifoIO, "1.0")
