/*
 * USBSignalDisplay.cpp
 *
 *  Created on: 16/ago/2016
 *      Author: pc
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "USBSignalDisplay.h"

#include "AdvancedErrorManagement.h"
#include "AnyType.h"
#include "FastPollingMutexSem.h"
#include "MemoryMapOutputBroker.h"
#include "Threads.h"
#include "TypeDescriptor.h"
#include "usbd_cdc_custom.h"
#include "usb_device.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static void USBThreadFunction(USBSignalDisplay &USBSignalDisplay) {

    StreamString title;
    title.Printf("MARTe2 Demo! USBSignalDisplay [%s]\n\r", USBSignalDisplay.GetName());
    title.Seek(0u);
    uint32_t size = title.Size() + 1;
    USBWrite(title.Buffer(), &size, (USBSignalDisplay.blocking + 1) % 2);

    uint32 numberOfSignals = USBSignalDisplay.GetNumberOfSignals();
    while (USBSignalDisplay.running) {
        Sleep::Sec(USBSignalDisplay.refreshTimeUS * 1e-6);
        uint32 n;
        for (n = 0u; n < numberOfSignals; n++) {
            StreamString line;
            line.Printf("%-20s : %#!\n\r", USBSignalDisplay.usbSignalInfos[n].signalName, USBSignalDisplay.usbSignalInfos[n].asAnyType);
            line.Seek(0);
            size = line.Size() + 1;
            USBWrite(line.Buffer(), &size, (USBSignalDisplay.blocking + 1) % 2);

            USBSignalDisplay.usbSignalInfos[n].signalName.Seek(0);
        }

        StreamString line;
        uint64 freq = HighResolutionTimer::Frequency();
        line.Printf("%-20s : %#!\n\r", "Frequency", freq);
        line.Seek(0);
        size = line.Size() + 1;
        USBWrite(line.Buffer(), &size, (USBSignalDisplay.blocking + 1) % 2);
        n++;
        line = "";
        float64 period = HighResolutionTimer::Period();
        line.Printf("%-20s : %#!\n\r", "Period", period);
        line.Seek(0);
        size = line.Size() + 1;
        USBWrite(line.Buffer(), &size, (USBSignalDisplay.blocking + 1) % 2);
        n++;
        line = "";
        line.Seek(0);
        uint32 counter = HighResolutionTimer::Counter32();
        line.Printf("%-20s : %#!\n\n\r", "Counter", counter);
        size = line.Size() + 1;
        USBWrite(line.Buffer(), &size, (USBSignalDisplay.blocking + 1) % 2);
    }
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
USBSignalDisplay::USBSignalDisplay() :
        DataSourceI() {
    usbSignalInfos = NULL;
    running = false;
    blocking = 1u;
    activate = 1u;
}

USBSignalDisplay::~USBSignalDisplay() {
    if (usbSignalInfos != NULL) {
        uint32 n;
        for (n = 0u; n < GetNumberOfSignals(); n++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(usbSignalInfos[n].signalValue);
        }
        delete[] usbSignalInfos;
    }

}

bool USBSignalDisplay::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        float32 frameFreq;
        if (ret) {
            if (!data.Read("FrameRate", frameFreq)) {
                REPORT_ERROR(ErrorManagement::Warning, "FrameRate not set: used default 1Hz");
                frameFreq = 1.;
            }

            if (!data.Read("Blocking", blocking)) {
                REPORT_ERROR(ErrorManagement::Warning, "Blocking not set: used default 1 (true)");
                blocking = 1u;
            }

            if (!data.Read("Activate", activate)) {
                REPORT_ERROR(ErrorManagement::Warning, "Activate not set: used default 1 (true)");
                activate = 1u;
            }
        }
        if (ret) {
            refreshTimeUS = (1e6 / frameFreq);
        }
    }
    return ret;
}

bool USBSignalDisplay::Synchronise() {
    return true;
}

bool USBSignalDisplay::AllocateMemory() {
    return true;
}

uint32 USBSignalDisplay::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool USBSignalDisplay::GetSignalMemoryBuffer(const uint32 signalIdx,
                                             const uint32 bufferIdx,
                                             void *&signalAddress) {
    signalAddress = usbSignalInfos[signalIdx].signalValue;
    return true;
}

const char8 *USBSignalDisplay::GetBrokerName(StructuredDataI &data,
                                             const SignalDirection direction) {
    if (direction == OutputSignals) {
        return "MemoryMapOutputBroker";
    }
    return "";
}

bool USBSignalDisplay::GetInputBrokers(ReferenceContainer &inputBrokers,
                                       const char8* const functionName,
                                       void * const gamMemPtr) {
    return false;
}

bool USBSignalDisplay::GetOutputBrokers(ReferenceContainer &outputBrokers,
                                        const char8* const functionName,
                                        void * const gamMemPtr) {
    ReferenceT < MemoryMapOutputBroker > broker("MemoryMapOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool USBSignalDisplay::SetConfiguredDatabase(StructuredDataI & data) {

    bool ret = DataSourceI::SetConfiguredDatabase(data);
    uint32 numberOfSignals = GetNumberOfSignals();

    if (ret) {
        if (numberOfSignals > 0u) {
            usbSignalInfos = new SignalInfo[numberOfSignals];
            uint32 i;
            for (i = 0u; i < numberOfSignals; i++) {
                GetSignalName(i, usbSignalInfos[i].signalName);
                usbSignalInfos[i].signalName.Seek(0u);
                TypeDescriptor desc = GetSignalType(i);
                usbSignalInfos[i].signalValue = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(desc.numberOfBits / 8u);
                AnyType toAssign(desc, 0, usbSignalInfos[i].signalValue);
                usbSignalInfos[i].asAnyType = toAssign;
            }
        }

        if (activate) {
            //Initialisation already done by the tool !!!
            if (!USBInitialized()) {
                USBOpen();
            }
            running = true;
        }

        if (running) {
            Threads::BeginThread((ThreadFunctionType) USBThreadFunction, this, 512);
        }

    }

    return ret;
}

bool USBSignalDisplay::PrepareNextState(const MARTe::char8 * const currentStateName,
                                        const MARTe::char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(USBSignalDisplay, "1.0")
