/**
 * @file UdpSenderDataSource.cpp
 * @brief Source file for class ADCDMA
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
 * the class UdpSenderDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UdpSenderDataSource.h"
#include "ObjectRegistryDatabase.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

UdpSenderDataSource::UdpSenderDataSource() :
        MemoryDataSourceI(), udpInterface() {
//Initialisation already done by the tool !!!
}

UdpSenderDataSource::~UdpSenderDataSource() {
}

bool UdpSenderDataSource::Initialise(StructuredDataI &data) {
    bool ret = MemoryDataSourceI::Initialise(data);
    if(ret){
        StreamString interfacePath;
        ret = data.Read("Interface", interfacePath);
        if(ret){
            udpInterface=ObjectRegistryDatabase::Instance()->Find(interfacePath.Buffer());
            ret=udpInterface.IsValid();
            if(!ret){
                REPORT_ERROR(ErrorManagement::FatalError, "Interface %s not valid", interfacePath.Buffer());
            }
        }
        else{
            REPORT_ERROR(ErrorManagement::InitialisationError, "Interface not specified");
        }

    }
    if (ret) {
        Vector < uint8 > ip(4);
        uint16 port;
        ret = data.Read("IpAddress", ip);
        if (ret) {
            ret = data.Read("Port", port);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Port not specified");
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Failed to read IpAddress");
        }
        if (ret) {
            ret = udpInterface->Connect(ip.GetDataPointer(), port);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::FatalError, "Failed UdpStream::Connect");
            }
        }
    }
    return ret;
}

bool UdpSenderDataSource::Synchronise() {
    return udpInterface->Write((const char8 *)memory, totalMemorySize, 0xFFFFFFFFu);
}

bool UdpSenderDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = MemoryDataSourceI::SetConfiguredDatabase(data);
    return ret;
}

const char8* UdpSenderDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == OutputSignals) {
        return "MemoryMapSynchronisedOutputBroker";
    }

    return "";
}

bool UdpSenderDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {

    return true;

}

CLASS_REGISTER(UdpSenderDataSource, "1.0")
