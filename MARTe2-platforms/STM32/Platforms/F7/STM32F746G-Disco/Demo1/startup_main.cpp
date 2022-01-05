/**
 * @file main.c
 * @brief Source file for class main.c
 * @date 10/08/2016
 * @author Andre' Neto
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
 * the class BufferGAM (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "BasicScheduler.h"
#include "GAM.h"
#include "GAMGroup.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeThread.h"
#include "StandardParser.h"
#include "StreamMemoryReference.h"
#include "Threads.h"

using namespace MARTe;

#define INCLUDE_CFG_FILE(x) QUOTE(x)

const char8 *const config = 
#include INCLUDE_CFG_FILE(__CFG__FILE__)
;


StreamString allErrors;
void DebugErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                               const char * const errorDescription) {
    allErrors += errorDescription;
    allErrors += " ";
}

static void MARTeAppLauncher(void const *ignored) {

    uint32 confSize = StringHelper::Length(config) + 1;
    ConfigurationDatabase cdb;
    StreamMemoryReference *stream = new StreamMemoryReference(config, confSize);
    stream->Seek(0);
    StandardParser parser(*stream, cdb);
    bool ok = parser.Parse();

    ObjectRegistryDatabase *godb = NULL;
    if(ok) {
        godb = ObjectRegistryDatabase::Instance();
        godb->CleanUp();
    }

    if(ok) {
        ok = godb->Initialise(cdb);
    }
    ReferenceT < RealTimeApplication > application;
    if (ok) {
        application = godb->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    if (ok) {
        ok = application->PrepareNextState("State1");
    }
    if (ok) {
        application->StartExecution();
        ReferenceT<BasicScheduler> scheduler = godb->Find("Application1.Scheduler");
        if (scheduler.IsValid()) {
            while(1) {
                scheduler->Cycle(0);
            }
        }
    }
    delete stream;
}

extern "C" {
int main(int argc, char **argv) {
    SetErrorProcessFunction(&DebugErrorProcessFunction);
    Threads::BeginThread(MARTeAppLauncher, NULL, configMINIMAL_STACK_SIZE * 16); 
    while(1){
        Sleep::Sec(10.0);
    }
}
}

