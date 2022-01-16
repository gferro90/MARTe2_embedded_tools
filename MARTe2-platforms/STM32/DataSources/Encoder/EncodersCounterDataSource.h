/*
 * EncodersCounterDataSource.h
 *
 *  Created on: 31/ago/2016
 *      Author: pc
 */

#ifndef ENCODERSCOUNTERDATASOURCE_H_
#define ENCODERSCOUNTERDATASOURCE_H_
#include "TIMBase.h"
#include QUOTE(_HAL_H)

using namespace MARTe;
//TODO The ...GetState() function returns the initialisation handle and we can change parameters and re-initialise.
//TODO Interrupt and DMA cases (but check before if it works in polling mode)
class EncodersCounterDataSource: public TIMBase {
public:

	CLASS_REGISTER_DECLARATION()

EncodersCounterDataSource();

	virtual ~EncodersCounterDataSource();

	virtual bool Initialise(StructuredDataI &data);

    virtual bool SetConfiguredDatabase(MARTe::StructuredDataI & data);

	virtual TIM_HandleTypeDef * GetHandle();

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);
private:
	TIM_HandleTypeDef *encoderHandlePtr ;
	uint8 resetEncoder;
};

#endif /* ENCODERSCOUNTERDATASOURCE_H_ */
