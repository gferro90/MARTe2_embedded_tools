/**
 * @file GenericPeripheral.cpp
 * @brief Source file for class GenericPeripheral
 * @date 19 giu 2018
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
 * the class GenericPeripheral (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "GenericPeripheral.h"
#include "GenericPeriphReader.h"
#include "GenericPeriphWriter.h"
#include "GenericPeriphSyncReader.h"
#include "GenericPeriphSyncWriter.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"
#include "MemoryMapMultiBufferInputBroker.h"
#include "MemoryMapMultiBufferOutputBroker.h"
#include "MemoryMapSynchronisedMultiBufferInputBroker.h"
#include "MemoryMapSynchronisedMultiBufferOutputBroker.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GenericPeripheral::GenericPeripheral() :
        MemoryDataSourceI() {
    // Auto-generated constructor stub for GenericPeripheral
    // TODO Verify if manual additions are needed
    isBufferised = 0u;
}

GenericPeripheral::~GenericPeripheral() {
    // Auto-generated destructor stub for GenericPeripheral
    // TODO Verify if manual additions are needed
}

bool GenericPeripheral::AllocateMemory() {
    if (isBufferised) {
        return MemoryDataSourceI::AllocateMemory();
    }
    return true;
}

bool GenericPeripheral::Initialise(StructuredDataI & data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        if (!data.Read("IsBufferised", isBufferised)) {
            isBufferised = 0u;
        }
    }
    return ret;
}

const char8 *GenericPeripheral::GetBrokerName(StructuredDataI &data,
                                              const SignalDirection direction) {

    const char8* brokerName = NULL_PTR(const char8 *);
    float32 frequency = 0.F;
    if (!data.Read("Frequency", frequency)) {
        frequency = 0.F;
    }
    uint32 trigger = 0u;
    if (!data.Read("Trigger", trigger)) {
        trigger = 0u;
    }
    if (direction == InputSignals) {
        syncInputBrokerName = "GenericPeriphSyncReader";

        if (isBufferised) {
            if (GetNumberOfMemoryBuffers() == 1u) {
                if ((trigger > 0u) || (frequency >= 0.F)) {
                    brokerName = "MemoryMapSynchronisedInputBroker";
                    syncInputBrokerName = brokerName;
                }
                else {
                    brokerName = "MemoryMapInputBroker";
                }
            }
            else {
                if ((trigger > 0u) || (frequency > 0.F)) {
                    brokerName = "MemoryMapSynchronisedMultiBufferInputBroker";
                    syncInputBrokerName = brokerName;
                }
                else {
                    brokerName = "MemoryMapMultiBufferInputBroker";
                }
            }
        }
        else {

            if ((trigger > 0u) || (frequency >= 0.F)) {
                brokerName = "GenericPeriphSyncReader";
            }
            else {
                brokerName = "GenericPeriphReader";
            }
        }
    }

    if (direction == OutputSignals) {
        syncOutputBrokerName = "GenericPeriphSyncWriter";

        if (isBufferised) {
            if (GetNumberOfMemoryBuffers() == 1u) {
                if ((trigger > 0u) || (frequency >= 0.F)) {
                    brokerName = "MemoryMapSynchronisedOutputBroker";
                    syncOutputBrokerName = brokerName;
                }
                else {
                    brokerName = "MemoryMapOutputBroker";
                }
            }
            else {
                if ((trigger > 0u) || (frequency > 0.F)) {
                    brokerName = "MemoryMapSynchronisedMultiBufferOutputBroker";
                    syncOutputBrokerName = brokerName;
                }
                else {
                    brokerName = "MemoryMapMultiBufferOutputBroker";
                }
            }
        }
        else {
            if ((trigger > 0u) || (frequency >= 0.F)) {
                brokerName = "GenericPeriphSyncWriter";
            }
            else {
                brokerName = "GenericPeriphWriter";
            }
        }

    }

    return brokerName;
}

bool GenericPeripheral::GetSignalMemoryBuffer(const uint32 signalIdx,
                                              const uint32 bufferIdx,
                                              void *&signalAddress) {
    if (isBufferised) {
        return MemoryDataSourceI::GetSignalMemoryBuffer(signalIdx, bufferIdx, signalAddress);
    }
    return true;
}

bool GenericPeripheral::Synchronise(){
    return true;
}


bool GenericPeripheral::IsBufferised() {
    return isBufferised;
}
