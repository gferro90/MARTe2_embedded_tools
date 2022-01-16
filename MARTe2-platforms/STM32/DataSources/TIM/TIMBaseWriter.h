/*
 * TIMBaseWriter.h
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#ifndef TIMBASEDATASOURCEWRITER_H_
#define TIMBASEDATASOURCEWRITER_H_

#include "MemoryMapInputBroker.h"
#include QUOTE(_HAL_H)

using namespace MARTe;

class TIMBaseWriter : public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

TIMBaseWriter();
	virtual ~TIMBaseWriter();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress);

private:
	TIM_HandleTypeDef *timHandle;
};


#endif /* TIMBASEDATASOURCEWRITER_H_ */
