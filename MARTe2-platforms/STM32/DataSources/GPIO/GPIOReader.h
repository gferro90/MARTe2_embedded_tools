/*
 * GPIOReader.h
 *
 *  Created on: 30/ago/2016
 *      Author: pc
 */

#ifndef GPIO_GPIOREADER_H_
#define GPIO_GPIOREADER_H_

#include "MemoryMapInputBroker.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

class GPIOReader: public MemoryMapInputBroker {
public:
	CLASS_REGISTER_DECLARATION()

	GPIOReader();
	virtual ~GPIOReader();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
			DataSourceI &dataSourceIn, const char8 * const functionName,
			void * const gamMemoryAddress);

private:
	GPIO_TypeDef *gpioHandle;
};

#endif /* GPIO_GPIOREADER_H_ */
