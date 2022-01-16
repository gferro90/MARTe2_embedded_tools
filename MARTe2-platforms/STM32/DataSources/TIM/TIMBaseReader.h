/*
 * TIMBaseReader.h
 *
 *  Created on: 29/ago/2016
 *      Author: pc
 */

#ifndef TIM_TIMBASEDATASOURCEREADER_H_
#define TIM_TIMBASEDATASOURCEREADER_H_


#include "MemoryMapInputBroker.h"
#include QUOTE(_HAL_H)


using namespace MARTe;


class TIMBaseReader: public MemoryMapInputBroker {
public:
    CLASS_REGISTER_DECLARATION()

TIMBaseReader();
	virtual ~TIMBaseReader();
	virtual bool Execute();

	virtual bool Init(const SignalDirection direction,
	                           DataSourceI &dataSourceIn,
	                           const char8 * const functionName,
	                           void * const gamMemoryAddress);

protected:
	TIM_HandleTypeDef *timHandle;
};

#endif /* TIM_TIMBASEDATASOURCEREADER_H_ */
