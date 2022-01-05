/**
 * @file CfgUploader.cpp
 * @brief Source file for class CfgUploader
 * @date Jun 16, 2017
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
 * the class CfgUploader (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CfgUploader.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

CfgUploader::CfgUploader() :
        ReferenceContainer() {

    packetSize = 0u;

}

CfgUploader::~CfgUploader() {
    // Auto-generated destructor stub for CfgUploader
    // TODO Verify if manual additions are needed
}

bool CfgUploader::Initialise(StructuredDataI &data) {

    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        if (!data.Read("InitialSequence", initialSequence)) {
            initialSequence = "1234";
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Undefined InitialSequence, using default %s", initialSequence.Buffer());
        }
        if (!data.Read("FinalSequence", finalSequence)) {
            finalSequence = "1234";
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Undefined FinalSequence, using default %s", finalSequence.Buffer());
        }
        if (!data.Read("PacketSize", packetSize)) {
            packetSize = 64u;
            REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Undefined PacketSize, using default %d", packetSize);
        }

        stream = Find("Stream");
        ret = stream.IsValid();
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid stream");
        }

    }
    return ret;
}

bool CfgUploader::UploadCfg(StreamString &cfgBuffer) {
    //read the initial sequence
    //REPORT_ERROR(ErrorManagement::Warning, "Reading Initial Sequence...");

    cfgBuffer = "";
    uint32 sizeToRead = packetSize;
    char8* buffer = new char8[packetSize + 1];
    bool ret = (buffer != NULL);
    if (ret) {
        MemoryOperationsHelper::Set(buffer, 0, packetSize + 1);
        stream->Read(buffer, sizeToRead, (uint32)(-1));

        buffer[packetSize] = 0;
        //REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Read %d %s", sizeToRead, buffer);
        ret = (initialSequence == buffer);
        if (ret) {
            REPORT_ERROR(ErrorManagement::Warning, "Read Initial Sequence");
            MemoryOperationsHelper::Set(buffer, 0, packetSize + 1);
            for (;;) {
                sizeToRead = packetSize;
                stream->Read(buffer, sizeToRead, (uint32)(-1));
                buffer[packetSize] = 0;
                REPORT_ERROR_PARAMETERS(ErrorManagement::Warning, "Read %d", sizeToRead);

                if (finalSequence == buffer) {
                    REPORT_ERROR(ErrorManagement::Warning, "Read Final Sequence");
                    break;
                }
                cfgBuffer += buffer;
                MemoryOperationsHelper::Set(buffer, 0, packetSize + 1);
            }
        }

        delete[] buffer;
    }
    return ret;
}

CLASS_REGISTER(CfgUploader, "1.0")
