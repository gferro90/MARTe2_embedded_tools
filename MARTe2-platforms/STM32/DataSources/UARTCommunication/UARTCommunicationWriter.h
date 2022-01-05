/*
 * UARTCommunicationWriter.h
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#ifndef UARTCOMMUNICATIONWRITER_H_
#define UARTCOMMUNICATIONWRITER_H_

#include "MemoryMapOutputBroker.h"
#include QUOTE(_HAL_H)

using namespace MARTe;


class UARTCommunicationWriter : public MemoryMapOutputBroker{
public:
    CLASS_REGISTER_DECLARATION()

	UARTCommunicationWriter();
	virtual ~UARTCommunicationWriter();
	virtual bool Execute();
	virtual bool Init(const SignalDirection direction,
			DataSourceI &dataSourceIn, const char8 * const functionName,
			void * const gamMemoryAddress);

private:
	UART_HandleTypeDef *uartHandle;

};

#endif /* UARTCOMMUNICATIONWRITER_H_ */
