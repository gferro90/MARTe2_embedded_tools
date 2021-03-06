/**
 * @file UdpStream.h
 * @brief Header file for class UdpStream
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

 * @details This header file contains the declaration of the class UdpStream
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#ifndef UDPSTREAM_H_
#define UDPSTREAM_H_
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamParent.h"
#include "UdpCore.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class UdpStream: public StreamParent {
public:
    CLASS_REGISTER_DECLARATION()

    UdpStream();
    virtual ~UdpStream();

    virtual bool Initialise(StructuredDataI &data);

    virtual bool Read(char8 * const output,
                      uint32 & size,
                      uint32 timeout);
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       uint32 timeout);

private:
    UdpCore udpCore;
};

#endif /* UDPSTREAM_H_ */
