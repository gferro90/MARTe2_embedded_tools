/*
 * FifoAcquisition.cpp
 *
 *  Created on: Feb 14, 2018
 *      Author: codac-dev
 */

#include "FifoAcquisition.h"
#include "AdvancedErrorManagement.h"

FifoAcquisition::FifoAcquisition() :
		EfficientDriver() {
	// TODO Auto-generated constructor stub

	fifoDev = 0;
	store = 0;
}

FifoAcquisition::~FifoAcquisition() {
	// TODO Auto-generated destructor stub
	NiFpga_Status status = NiFpga_Abort(session);

	/* Close the session now that we're done with it. */
	NiFpga_MergeStatus(&status, NiFpga_Close(session, 0));

}

bool FifoAcquisition::Initialise(StructuredDataI & data) {
	bool ret = EfficientDriver::Initialise(data);
	if (ret) {
		StreamString niRioDeviceName;
		ret = data.Read("NiRioDeviceName", niRioDeviceName);
		if (!ret) {
			REPORT_ERROR(ErrorManagement::InitialisationError,
					"Please spacify the NI-RIO Device name (NiRioDeviceName)");
		}
		StreamString niRioGenFile;
		if (ret) {

			ret = data.Read("NiRioGenFile", niRioGenFile);
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Please spacify the NI-RIO generated bit file (NiRioGenFile)");
			}
		}
		StreamString niRioGenSignature;
		if (ret) {

			ret = data.Read("NiRioGenSignature", niRioGenSignature);
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Please spacify the NI-RIO generated signature (NiRioGenSignature)");
			}
		}
		if (ret) {
			StreamString fifoName;
			ret = data.Read("FifoName", fifoName);
			if (!ret) {
				REPORT_ERROR(ErrorManagement::InitialisationError,
						"Please spacify the FIFO name (FifoName)");
			}

			REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Opening %s",
					niRioDeviceName.Buffer());

			NiFpga_Status status = NiFpga_Open(niRioGenFile.Buffer(),
					niRioGenSignature.Buffer(), niRioDeviceName.Buffer(),
					NiFpga_OpenAttribute_NoRun, &session);
			ret = (status == 0);
			if (ret) {
				fifoDev = SearchLabviewGen(fifoName.Buffer(), ret);
				if (!ret) {
					REPORT_ERROR_PARAMETERS(
							ErrorManagement::InitialisationError,
							"Fifo name %s not found", fifoName.Buffer());
				}
			} else {
				REPORT_ERROR_PARAMETERS(ErrorManagement::InitialisationError,
						"Opening FPGA failed status=%d", status);
			}
		}
	}

	return ret;
}

bool FifoAcquisition::PrepareNextState(const char8 * const currentStateName,
		const char8 * const nextStateName) {
	bool ret = EfficientDriver::PrepareNextState(currentStateName,
			nextStateName);
	if (ret) {
		uint32 hostFifoSize = (readBufferSize * numberOfReadBuffers)
				/ sizeof(int32);
		NiFpga_Status status = NiFpga_ConfigureFifo(session, fifoDev,
				hostFifoSize);
		ret = (status == 0);
		if (!ret) {
			REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
					"Failed to configure host FIFO size to %d, status=%d %d",
					hostFifoSize, status, numberOfReadBuffers);
		}
	}
	if (ret) {
		REPORT_ERROR(ErrorManagement::Information, "Running...");
		NiFpga_Status status;
		NiFpga_MergeStatus(&status, NiFpga_Run(session, 0));
		ret = (status == 0);
		if (ret) {
			REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError,
					"Failed to Run the application, status=%d", status);
		}
	}
	return ret;
}

bool FifoAcquisition::Synchronise() {
	uint32 fifoRemaining = 0u;

	readBufferIndex++;
	readBufferIndex %= numberOfReadBuffers;
	//todo configurable timeout
	NiFpga_Status status = NiFpga_ReadFifoI32(session, fifoDev,
			(int32*) (&readMem[readBufferIndex * readBufferSize]),
			readBufferSize / sizeof(int32), 0xFFFFFFFF,
			(size_t*) (&fifoRemaining));

	uint32 current = *(int32*) (&readMem[readBufferIndex * readBufferSize]);
	/*REPORT_ERROR_PARAMETERS("Read %d remaining=%d status=%d gap=%d", current,
			fifoRemaining, status, (current - store));*/
	REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Read %d", current);
	store = current;
	return (status == 0);
}

CLASS_REGISTER(FifoAcquisition, "1.0")
