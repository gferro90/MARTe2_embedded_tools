/**
 * @file ADC_DMAReader.cpp
 * @brief Source file for class ADC_DMAReader
 * @date 28/set/2016
 * @author pc
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class ADC_DMAReader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ADC_DMA.h"
#include "ADC_DMAReader.h"
#include "AdvancedErrorManagement.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static volatile int32 conversionCompleted = 0u;

extern "C" {
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle) {
    Atomic::Increment(&conversionCompleted);
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ADC_DMAReader::ADC_DMAReader() :
        MemoryMapInputBroker() {
// TODO Auto-generated constructor stub
    AdcHandle = NULL;
    conversionCompleted = 0u;
}

ADC_DMAReader::~ADC_DMAReader() {
// TODO Auto-generated destructor stub
}

bool ADC_DMAReader::Execute() {
    uint32 n;
    bool ret = true;
    for (n = 0u; (n < numberOfCopies) && (ret); n++) {
        if (copyTable != NULL_PTR(MemoryMapBrokerCopyTableEntry *)) {
            HAL_ADC_Start_DMA(AdcHandle, (uint32_t *) copyTable[n].gamPointer, (copyTable[n].copySize) / sizeof(uint32_t));
            //Sleep::MSec(10); //todo
            while (!conversionCompleted)
                ;
            Atomic::Decrement(&conversionCompleted);
            HAL_ADC_Stop_DMA(AdcHandle);

        }
    }
    return ret;
}

bool ADC_DMAReader::Init(const SignalDirection direction,
                         DataSourceI &dataSourceIn,
                         const char8 * const functionName,
                         void * const gamMemoryAddress) {
    bool ret = MemoryMapInputBroker::Init(direction, dataSourceIn, functionName, gamMemoryAddress);
    ADC_DMA *adcDS = dynamic_cast<ADC_DMA *>(dataSource);
    if (ret) {
        ret = (adcDS != NULL);
    }
    if (ret) {
        AdcHandle = adcDS->GetHwHandle();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "Dynamic cast from DataSourceI to ADC_DMA failed");
    }
    return ret;
}

CLASS_REGISTER(ADC_DMAReader, "1.0")
