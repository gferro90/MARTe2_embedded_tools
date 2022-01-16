/*
 * GPIOWriter.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef GPIO_GPIOWRITER_H_
#define GPIO_GPIOWRITER_H_
#include "MemoryMapInputBroker.h"
#include QUOTE(_HAL_H)

using namespace MARTe;
class GPIOWriter : public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

GPIOWriter();
	virtual ~GPIOWriter();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress);

private:
	GPIO_TypeDef *gpioHandle;
};

#endif /* GPIOWRITER_H_ */
