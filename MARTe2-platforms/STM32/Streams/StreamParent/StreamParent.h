/**
 * @file StreamParent.h
 * @brief Header file for class StreamParent
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

 * @details This header file contains the declaration of the class StreamParent
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STREAMPARENT_H_
#define STREAMPARENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Interface of a stream communication device, used by the embedded board to obtain the configuration data.
 *
 * @details This object provices two basic interfaces to read and write from/to the device that must be customly
 * implemented.
 */
class StreamParent: public ReferenceContainer {
public:
    /**
     * @brief Constructor
     */
    StreamParent() {

    }

    /**
     * @brief Destructor
     */
    virtual ~StreamParent() {

    }

    /**
     * @brief Reads from the communication device.
     * @param[out] output a buffer that will contain the data read from the communication interface.
     * @param[in,out] size the number of bytes to be read. After the read operation, it is refreshed with the
     * number of bytes effectively read.
     * @param[in] timeout the timeout of the read operation.
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      uint32 timeout)=0;

    /**
     * @brief Writes the communication device.
     * @param[in] input a buffer that contains the data to be written to the communication interface.
     * @param[in,out] size the number of bytes to be written. After the write operation, it is refreshed with the
     * number of bytes effectively written.
     * @param[in] timeout the timeout of the write operation.
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       uint32 timeout)=0;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MARTE2_PLATFORMS_STM32_F4_PLATFORMS_STM32F407_VGT6_STREAMS_STREAMPARENT_STREAMPARENT_H_ */

