/*
 * EfficientWriterGamOutPtr.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef EfficientWriterGamOutPtr_H_
#define EfficientWriterGamOutPtr_H_



#include "MemoryMapOutputBroker.h"

using namespace MARTe;

class EfficientDriver;

class EfficientWriterGamOutPtr : public MemoryMapOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    EfficientWriterGamOutPtr();
    virtual ~EfficientWriterGamOutPtr();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    EfficientDriver *dataSourceCpuFpga;
};

#endif /* EfficientWriterGamOutPtr_H_ */

