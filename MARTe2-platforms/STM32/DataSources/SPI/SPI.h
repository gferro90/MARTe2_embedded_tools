/**
 * @file SPI.h
 * @brief Header file for class SPI
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

 * @details This header file contains the declaration of the class SPI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SPI_H_
#define SPI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class SPI {
public:

    CLASS_REGISTER_DECLARATION()

    SPI();
    virtual ~SPI();
    virtual bool Initialise(StructuredDataI &data);

    bool Synchronise();

    bool AllocateMemory();

    uint32 GetNumberOfMemoryBuffers();

    bool GetSignalMemoryBuffer(const uint32 signalIdx,
                               const uint32 bufferIdx,
                               void *&signalAddress);

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers,
                                 const char8* const functionName,
                                 void * const gamMemPtr);

    virtual bool GetOutputBrokers(ReferenceContainer &outputBrokers,
                                  const char8* const functionName,
                                  void * const gamMemPtr);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SPI_H_ */

