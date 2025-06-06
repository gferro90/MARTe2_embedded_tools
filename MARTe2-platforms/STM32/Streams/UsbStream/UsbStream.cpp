/**
 * @file UsbStream.cpp
 * @brief Source file for class UsbStream
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
 * the class UsbStream (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "UsbStream.h"
#include "usbd_cdc_custom.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

UsbStream::UsbStream() {
    if (!USBInitialized()) {
        USBOpen();
    }
    USBSync();
}

UsbStream::~UsbStream() {
    // Auto-generated destructor stub for UsbStream
    // TODO Verify if manual additions are needed
}

bool UsbStream::Read(char8 * const output,
                     uint32 & size,
                     uint32 timeout) {
    USBRead(output, (uint32_t*) &size, (timeout == 0));
    USBSync();
}
bool UsbStream::Write(const char8 * const input,
                      uint32 & size,
                      uint32 timeout) {
    USBWrite(input, (uint32_t*) &size, (timeout == 0));
    USBSync();
}
CLASS_REGISTER(UsbStream, "1.0")
