/*
 * EfficientWriter.h
 *
 *  Created on: Jan 22, 2018
 *      Author: gferro
 */

#ifndef EfficientWriter_H_
#define EfficientWriter_H_



#include "MemoryMapOutputBroker.h"

using namespace MARTe;

class EfficientDriver;

class EfficientWriter : public MemoryMapOutputBroker {
public:
    CLASS_REGISTER_DECLARATION()

    EfficientWriter();
    virtual ~EfficientWriter();
    virtual bool Execute();
    virtual bool Init(const SignalDirection direction,
                      DataSourceI &dataSourceIn,
                      const char8 * const functionName,
                      void * const gamMemoryAddress);

private:
    EfficientDriver *dataSourceCpuFpga;

};

#endif /* EfficientWriter_H_ */

