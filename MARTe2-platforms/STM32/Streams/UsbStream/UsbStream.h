/**
 * @file UsbStream.h
 * @brief Header file for class UsbStream
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

 * @details This header file contains the declaration of the class UsbStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef USBSTREAM_H_
#define USBSTREAM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamParent.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Implementation of USB communication interface for STM32 boards.
 */
class UsbStream: public StreamParent {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @details Opens the USB device interface.
     */
    UsbStream();

    /**
     * @brief Destructor
     */
    virtual ~UsbStream();

    /**
     * @see StremParent::Read
     * @details Reads a buffer of \a size bytes from the STM32 board USB interface.
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      uint32 timeout);

    /**
     * @see StremParent::Write
     * @details Writes a buffer of \a size bytes to the STM32 board USB interface.
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       uint32 timeout);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* USBSTREAM_H_ */

