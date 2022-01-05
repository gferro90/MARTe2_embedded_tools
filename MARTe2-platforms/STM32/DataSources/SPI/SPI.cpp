/**
 * @file SPI.cpp
 * @brief Source file for class SPI
 * @date May 24, 2017
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
 * the class SPI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SPI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SPI::SPI() {
    // Auto-generated constructor stub for SPI
    // TODO Verify if manual additions are needed
}

SPI::~SPI() {
    // Auto-generated destructor stub for SPI
    // TODO Verify if manual additions are needed
}



SPI::SPI() :
        DataSourceI() {
}

SPI::~SPI() {

}

bool SPI::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {


    }

    return true;
}

bool SPI::Synchronise() {
    return true;
}

bool SPI::AllocateMemory() {
    return true;
}

uint32 SPI::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool SPI::GetSignalMemoryBuffer(const uint32 signalIdx,
                                             const uint32 bufferIdx,
                                             void *&signalAddress) {
    signalAddress = NULL;
    return true;
}

const char8 *SPI::GetBrokerName(StructuredDataI &data,
                                             const SignalDirection direction) {
    if (direction == InputSignals) {
        return "SPIReader";
    }
    if (direction == OutputSignals) {
        return "SPIWriter";
    }
    return "";
}

bool SPI::GetInputBrokers(ReferenceContainer &inputBrokers,
                                       const char8* const functionName,
                                       void * const gamMemPtr) {
    ReferenceT<SPIReader> broker("SPIReader");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(InputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = inputBrokers.Insert(broker);
    }
    return ret;
}

bool SPI::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                        const char8* const functionName,
                                        void * const gamMemPtr) {
    ReferenceT<SPIWriter> broker("SPIWriter");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool SPI::PrepareNextState(const MARTe::char8 * const currentStateName,
                                        const MARTe::char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(SPI, "1.0")

