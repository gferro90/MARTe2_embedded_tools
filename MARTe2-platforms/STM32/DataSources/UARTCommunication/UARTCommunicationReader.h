/*
 * UARTCommunicationReader.h
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#ifndef UARTCOMMUNICATIONREADER_H_
#define UARTCOMMUNICATIONREADER_H_

#include "MemoryMapInputBroker.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

class UARTCommunicationReader: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

UARTCommunicationReader();
	virtual ~UARTCommunicationReader();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress);

private:
	UART_HandleTypeDef *uartHandle;
};

#endif /* UARTCOMMUNICATIONREADER_H_ */
