/**
 * @file CfgUploader.h
 * @brief Header file for class CfgUploader
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

 * @details This header file contains the declaration of the class CfgUploader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CFGUPLOADER_H_
#define CFGUPLOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "StreamParent.h"
#include "StreamString.h"
#include "ReferenceT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Allows to send a configuration file trough a communication device.
 *
 * @details The user must configure the packet size (how many byte to read per operation), the communication device to be
 * used (it must be a StreamParent object) and an initial and final sequence to check when the significant data starts and ends.
 *
 * @The device that should receive the configuration from the extern world, must contain internally a local configuration file, very simple,
 * containing the configuration of the CfgUploader. Follows an example of a local configuration file, where the used stream to upload the
 * real configuration is the usb interface.
 * <pre>
 * +CfgUploader = {
 *   Class = CfgUploader
 *   InitialSequence = "1234" //the initial sequence asserts that follows significant data
 *    FinalSequence = "4321" //the final sequence asserts that all the data has been sent
 *    PacketSize = 64 //how many bytes to send per time
 *    +Stream = {
 *        Class = UsbStream //the stream interface
 *    }
 * }
 * </pre>
 */
class CfgUploader: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    CfgUploader();

    /**
     * @brief Destructor
     */
    virtual ~CfgUploader();

    /**
     * @see ReferenceContainer::Initialise
     * @details The user can configure the following parameters in the local configuration string that
     * is compiled within the code running on the embedded device:\n
     *   InitialSequence: a string with a pattern that asserts that follows the configuration data (default "1234")
     *   FinalSequenct: a string with a pattern that asserts that all the configuration data has been sent (default "4321")
     *   PacketSize: the number of bytes that the embedded device received each read operation. It must match the packet size
     *     of the sender. (default 64 bytes)
     *   Stream: the configuration of the device to be used by the embedded device to receive the configuration data. See StreamParent
     *     to see how this bock has to be configured.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Receives the configuration from the external world using the configured stream interface.
     * @param[out] cfgBuffer is a StreamString containing the received configuration.
     * @return true if the configuration data has been received correctly, false otherwise.
     */
    bool UploadCfg(StreamString &cfgBuffer);

protected:

    /**
     * Holds the initial sequence
     */
    StreamString initialSequence;

    /**
     * Holds the final sequence
     */
   StreamString finalSequence;

    /**
     * Holds the packet size
     */
    uint32 packetSize;

    /**
     * Holds the reference to the stream
     * interface to be used to receive the
     * configuration data
     */
    ReferenceT<StreamParent> stream;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CFGUPLOADER_H_ */

