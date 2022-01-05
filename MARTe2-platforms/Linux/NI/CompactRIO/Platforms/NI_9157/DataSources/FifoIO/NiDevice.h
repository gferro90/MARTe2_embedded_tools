/*
 * NiDevice.h
 *
 *  Created on: Feb 19, 2018
 *      Author: codac-dev
 */

#ifndef NIDEVSINGLETON_H_
#define NIDEVSINGLETON_H_

#include "ReferenceContainer.h"
#include "SearchLabviewGen.h"
#include "StreamString.h"


using namespace MARTe;

class NiDevice: public ReferenceContainer {
public:
	CLASS_REGISTER_DECLARATION()

	NiDevice();
	virtual ~NiDevice();
	virtual bool Initialise(StructuredDataI & data);

	virtual NiFpga_Status Open();
	virtual NiFpga_Status Run();

	uint8 IsOpened() const;
	uint8 IsRunning() const;

	NiFpga_Status NiRead(uint32 indicator, bool &value);

	NiFpga_Status NiRead(uint32 indicator, int8 &value);

	/**
	 * Reads an unsigned 8-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, uint8 &value);

	/**
	 * Reads a signed 16-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, int16 &value);

	/**
	 * Reads an unsigned 16-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, uint16 &value);

	/**
	 * Reads a signed 32-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, int32 &value);

	/**
	 * Reads an unsigned 32-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, uint32 &value);

	/**
	 * Reads a signed 64-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, int64 &value);

	/**
	 * Reads an unsigned 64-bit integer value from a given indicator or control.
	 *
	 * @param session handle to a currently open session
	 * @param indicator indicator or control from which to read
	 * @param value outputs the value that was read
	 * @return result of the call
	 */
	NiFpga_Status NiRead(uint32 indicator, uint64 &value);

	/**
	 * Writes a boolean value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, bool value);

	/**
	 * Writes a signed 8-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, int8 value);

	/**
	 * Writes an unsigned 8-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, uint8 value);

	/**
	 * Writes a signed 16-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, int16 value);

	/**
	 * Writes an unsigned 16-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, uint16 value);

	/**
	 * Writes a signed 32-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, int32 value);

	/**
	 * Writes an unsigned 32-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, uint32 value);

	/**
	 * Writes a signed 64-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, int64 value);

	/**
	 * Writes an unsigned 64-bit integer value to a given control or indicator.
	 *
	 * @param session handle to a currently open session
	 * @param control control or indicator to which to write
	 * @param value value to write
	 * @return result of the call
	 */
	NiFpga_Status NiWrite(uint32 control, uint64 value);

	NiFpga_Status NiConfigureFifo(uint32 fifo, uint32 requestedDepth,
			uint32 &actualDepth);

	/**
	 * Starts a FIFO. This method is optional.
	 *
	 * @param session handle to a currently open session
	 * @param fifo FIFO to start
	 * @return result of the call
	 */
	NiFpga_Status NiStartFifo(uint32 fifo);

	/**
	 * Stops a FIFO. This method is optional.
	 *
	 * @param session handle to a currently open session
	 * @param fifo FIFO to stop
	 * @return result of the call
	 */
	NiFpga_Status NiStopFifo(uint32 fifo);

	/**
	 * Reads from a target-to-host FIFO of signed 8-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, int8* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of unsigned 8-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, uint8* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of signed 16-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, int16* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of unsigned 16-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, uint16* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of signed 32-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, int32* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of unsigned 32-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, uint32* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of signed 64-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, int64* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);

	/**
	 * Reads from a target-to-host FIFO of unsigned 64-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo target-to-host FIFO from which to read
	 * @param data outputs the data that was read
	 * @param numberOfElements number of elements to read
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param elementsRemaining if non-NULL, outputs the number of elements
	 *                          remaining in the host memory part of the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiReadFifo(uint32 fifo, uint64* data, uint32 numberOfElements,
			uint32 timeout, uint32 &elementsRemaining);



	/**
	 * Writes to a host-to-target FIFO of signed 8-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const int8* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of unsigned 8-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const uint8* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of signed 16-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const int16* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of unsigned 16-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const uint16* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of signed 32-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const int32* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of unsigned 32-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const uint32* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of signed 64-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const int64* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	/**
	 * Writes to a host-to-target FIFO of unsigned 64-bit integers.
	 *
	 * @param session handle to a currently open session
	 * @param fifo host-to-target FIFO to which to write
	 * @param data data to write
	 * @param numberOfElements number of elements to write
	 * @param timeout timeout in milliseconds, or NiFpga_InfiniteTimeout
	 * @param emptyElementsRemaining if non-NULL, outputs the number of empty
	 *                               elements remaining in the host memory part of
	 *                               the DMA FIFO
	 * @return result of the call
	 */
	NiFpga_Status NiWriteFifo(uint32 fifo, const uint64* data,
			uint32 numberOfElements, uint32 timeout,
			uint32 &emptyElementsRemaining);

	NiFpga_Session GetSession();
private:
	NiFpga_Session session;
	NiFpga_Status status;
	uint8 isOpened;
	uint8 isRunning;
	StreamString niRioGenFile;
	StreamString niRioGenSignature;
	StreamString niRioDeviceName;
};

#endif /* NIDEVSINGLETON_H_ */
