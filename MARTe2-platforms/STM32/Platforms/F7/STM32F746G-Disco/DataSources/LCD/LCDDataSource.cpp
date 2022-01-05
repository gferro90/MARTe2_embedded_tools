/**
 * @file LCDDataSource.cpp
 * @brief Source file for class LCDDataSource
 * @date 6 Aug 2016
 * @author andre
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
 * the class LCDDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_hal_sdram.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_rcc.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "AnyType.h"
#include "FastPollingMutexSem.h"
#include "LCDDataSource.h"
#include "MemoryMapOutputBroker.h"
#include "Threads.h"
#include "TypeDescriptor.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#define LCD_FRAME_BUFFER          SDRAM_DEVICE_ADDR
struct LCDSignalInfo {
    StreamString signalName;
    void *signalValue;
    AnyType asAnyType;
};

static LCDSignalInfo *lcdSignalInfos;

static bool running;

static void LCDThreadFunction(LCDDataSource &lcdDataSource) {
    BSP_LCD_SetFont (&Font24);
    BSP_LCD_ClearStringLine(0);
    BSP_LCD_SetTextColor (LCD_COLOR_RED);
    StreamString title;
    uint32 startLine = 0u;
    title = "";
    title.Printf("MARTe2 Demo! LCDDataSource [%s]", lcdDataSource.GetName());
    title.Seek(0u);
    //BSP_LCD_DisplayStringAtLine(0, (uint8 *) title.Buffer());
    BSP_LCD_DisplayStringAtLine(0, (uint8 *) "Matilde");
    startLine++;
    title = "";
    uint32 cpuFreq = HAL_RCC_GetSysClockFreq() / 1e6;
    uint32 hrtFreq = static_cast<uint32>(HighResolutionTimer::Frequency());
    /*title.Printf("CPU Freq: %d MHz. HRT Freq: %d Hz", cpuFreq, hrtFreq);
    title.Seek(0u);
    BSP_LCD_DisplayStringAtLine(startLine, (uint8 *) title.Buffer());
    startLine++;*/

  /*  BSP_LCD_SetTextColor (LCD_COLOR_GREEN);
    uint32 numberOfSignals = lcdDataSource.GetNumberOfSignals();
    while (running) {
        StreamString line;
        Sleep::Sec(0.5);
        uint32 n;
        for (n = 0u; n < numberOfSignals; n++) {
            line = "";
            line.Printf("%-20s : %#!", lcdSignalInfos[n].signalName, lcdSignalInfos[n].asAnyType);
            line.Seek(0);
            BSP_LCD_ClearStringLine(n + startLine);
            BSP_LCD_DisplayStringAtLine(n + startLine, (uint8 *) (line.Buffer()));
            lcdSignalInfos[n].signalName.Seek(0);
        }
        n++;
        line = ""; 
        line.Seek(0);
        line.Printf("%-20s : %#!", "HRT", HighResolutionTimer::Counter32()); 
        BSP_LCD_DisplayStringAtLine(n + startLine, (uint8 *) (line.Buffer()));
    }
    */
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
LCDDataSource::LCDDataSource() :
        DataSourceI() {
    lcdSignalInfos = NULL;
    running = false;
}

LCDDataSource::~LCDDataSource() {
    if (lcdSignalInfos != NULL) {
        uint32 n;
        for(n=0u; n<GetNumberOfSignals(); n++) {
            GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(lcdSignalInfos[n].signalValue);
        }
        delete [] lcdSignalInfos;
    }

}

bool LCDDataSource::Synchronise() {
    return true;
}

bool LCDDataSource::AllocateMemory() {
    return true;
}

uint32 LCDDataSource::GetNumberOfMemoryBuffers() {
    return 1u;
}

bool LCDDataSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    signalAddress = lcdSignalInfos[signalIdx].signalValue;
    return true;
}

const char8 *LCDDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == OutputSignals) {
        return "MemoryMapOutputBroker";
    }
    return "";
}

bool LCDDataSource::GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
    return false;
}

bool LCDDataSource::GetOutputBrokers(ReferenceContainer &outputBrokers, const char8* const functionName, void * const gamMemPtr) {
    ReferenceT<MemoryMapOutputBroker> broker("MemoryMapOutputBroker");
    bool ret = broker.IsValid();
    if (ret) {
        ret = broker->Init(OutputSignals, *this, functionName, gamMemPtr);
    }
    if (ret) {
        ret = outputBrokers.Insert(broker);
    }
    return ret;
}

bool LCDDataSource::SetConfiguredDatabase(StructuredDataI & data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    uint32 numberOfSignals = GetNumberOfSignals();

    if (numberOfSignals > 0u) {
        lcdSignalInfos = new LCDSignalInfo[numberOfSignals];
        uint32 i;
        for (i = 0u; i < numberOfSignals; i++) {
            GetSignalName(i, lcdSignalInfos[i].signalName);
            lcdSignalInfos[i].signalName.Seek(0u);
            TypeDescriptor desc = GetSignalType(i);
            lcdSignalInfos[i].signalValue = GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(
                    desc.numberOfBits / 8u);
            AnyType toAssign(desc, 0, lcdSignalInfos[i].signalValue);
            lcdSignalInfos[i].asAnyType = toAssign;
        }
    }
    if (ret) {
        BSP_LCD_Init();
        BSP_LCD_LayerDefaultInit(0, (uint32_t) LCD_FRAME_BUFFER);
        //  BSP_LCD_SetLayerWindow(0, 0, 0, 240, 160);
        BSP_LCD_SetLayerVisible(0, ENABLE);

        BSP_LCD_SelectLayer(0);
        BSP_LCD_Clear (LCD_COLOR_BLACK);
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_SetTextColor (LCD_COLOR_GREEN);
        BSP_LCD_DisplayOn();

        running = true;
        Threads::BeginThread((ThreadFunctionType) LCDThreadFunction, this, configMINIMAL_STACK_SIZE * 4);
    }
    return ret;
}

bool LCDDataSource::PrepareNextState(const MARTe::char8 * const currentStateName, const MARTe::char8 * const nextStateName) {
    return true;
}

CLASS_REGISTER(LCDDataSource, "1.0")
