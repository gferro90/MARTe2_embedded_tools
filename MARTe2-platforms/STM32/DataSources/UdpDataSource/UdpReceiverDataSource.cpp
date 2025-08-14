/**
 * @file UdpReceiverDataSource.cpp
 * @brief Source file for class UDP Receiver
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
 * the class UdpReceiverDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UdpReceiverDataSource.h"
#include "ObjectRegistryDatabase.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

UdpReceiverDataSource::UdpReceiverDataSource() :
        MemoryDataSourceI(), udpInterface() {
//Initialisation already done by the tool !!!
}

UdpReceiverDataSource::~UdpReceiverDataSource() {
}

bool UdpReceiverDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if (ret) {
        StreamString interfacePath;
        ret = data.Read("Interface", interfacePath);
        if (ret) {
            udpInterface = ObjectRegistryDatabase::Instance()->Find(interfacePath.Buffer());
            ret = udpInterface.IsValid();
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "Interface %s not valid", interfacePath.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Interface not specified");
        }

    }
    return ret;
}

bool UdpReceiverDataSource::Synchronise() {
    uint32 size = totalMemorySize;
    return udpInterface->Read((char8*) memory, size, 0u);
}

bool UdpReceiverDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    return MemoryDataSourceI::SetConfiguredDatabase(data);
}

const char8* UdpReceiverDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    }

    return "";
}

bool UdpReceiverDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {

    return true;

}

CLASS_REGISTER(UdpReceiverDataSource, "1.0")
