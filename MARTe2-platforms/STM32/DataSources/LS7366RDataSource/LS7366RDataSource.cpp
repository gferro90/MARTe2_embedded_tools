/**
 * @file LS7366RDataSource.cpp
 * @brief Source file for class ADCDMA
 * @date 28/set/2016
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
 * the class LS7366RDataSource (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#include "LS7366RDataSource.h"
#include "AdvancedErrorManagement.h"
#include "HandleDatabase.h"

#define MDR0_REG (0x1 << 3)
#define MDR1_REG (0x2 << 3)
#define DTR_REG (0x3 << 3)
#define CNTR_REG (0x4 << 3)
#define OTR_REG (0x5 << 3)
#define STR_REG (0x6 << 3)

#define CLEAR (0x0 << 6)
#define READ (0x1 << 6)
#define WRITE (0x2 << 6)
#define LOAD (0x3 << 6)

#define X1 0x1
#define X2 0x2
#define X4 0x3

#define FREE (0x0 << 2)
#define SINGLE (0x1 << 2)
#define LIMIT (0x2 << 2)
#define MOD (0x3 << 2)

#define TIMER_U32 0x0
#define TIMER_U24 0x1
#define TIMER_U16 0x2
#define TIMER_U8 0x3

#define ENABLE (0x0 << 2)
#define DISABLE (0x1 << 2)

LS7366RDataSource::LS7366RDataSource() :
        DataSourceI() {
    //Initialisation already done by the tool !!!

    spiHandle = NULL;
    gpioSelector = NULL;
    selectorPin = 0u;
    reset = 0u;
    counter = 0u;
    invert = 0u;
}

LS7366RDataSource::~LS7366RDataSource() {
}

bool LS7366RDataSource::Initialise(StructuredDataI &data) {
    bool ret = DataSourceI::Initialise(data);
    if (ret) {
        StreamString spiId;
        ret = data.Read("SpiIdentifier", spiId);
        if (ret) {
            spiHandle = (SPI_HandleTypeDef*) GetHandle(spiId.Buffer());
            ret = (spiHandle != NULL);
            if (!ret) {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid SpiIdentifier %s", spiId.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::InitialisationError, "No SpiIdentifier specified");
        }
    }
    if (ret) {
        StreamString gpioId;
        if (data.Read("GpioIdentifier", gpioId)) {
            gpioSelector = (GPIO_TypeDef*) GetHandle(gpioId.Buffer());
            ret = (gpioSelector != NULL);
            if (ret) {
                ret = data.Read("SelectorPin", selectorPin);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "Please define SelectorPin");
                }
            } else {
                REPORT_ERROR(ErrorManagement::InitialisationError, "Invalid GpioIdentifier %s", gpioId.Buffer());
            }
        } else {
            REPORT_ERROR(ErrorManagement::Information, "No GpioIdentifier specified");
        }
    }
    if (ret) {
        //get the zero duty cycle
        if (!data.Read("Invert", invert)) {
            invert = 0u;
            REPORT_ERROR(ErrorManagement::Information, "Invert not defined. Set to default %d", invert);
        }
    }

    return ret;
}

bool LS7366RDataSource::SetConfiguredDatabase(MARTe::StructuredDataI &data) {
    bool ret = DataSourceI::SetConfiguredDatabase(data);
    if (ret) {
        ret = (numberOfSignals == 2u);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "Only two signals are admitted");
        }
    }
    if (ret) {
        ret = (GetSignalType(0u) == UnsignedInteger8Bit);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The LS7366RDataSource reset signal type must be uint8");
        }
    }
    if (ret) {
        ret = (GetSignalType(0u) == SignedInteger32Bit);
        if (!ret) {
            REPORT_ERROR(ErrorManagement::InitialisationError, "The LS7366RDataSource counter signal type must be int32");
        }
    }
    if (ret) {
        for (uint32 i = 0u; (i < numberOfSignals) && ret; i++) {

            uint8 nDims = 0u;
            ret = (GetSignalNumberOfDimensions(i, nDims));
            if (ret) {
                ret = (nDims == 0u);
                if (!ret) {
                    REPORT_ERROR(ErrorManagement::InitialisationError, "The LS7366RDataSource signal[%d] must be a scalar", i);
                }
            }
        }
    }
    if (ret) {
        if (gpioSelector != NULL) {
            HAL_GPIO_WritePin(gpioSelector, (1<<selectorPin), GPIO_PIN_RESET);
        }

        uint8 tx[] = {WRITE | MDR0_REG, X4};
        HAL_SPI_Transmit(spiHandle, tx , 2, HAL_MAX_DELAY);

        tx[0] = (WRITE | MDR1_REG);
        tx[1] = TIMER_U32;
        HAL_SPI_Transmit(spiHandle, tx , 2, HAL_MAX_DELAY);

        if(gpioSelector != NULL) {
            HAL_GPIO_WritePin(gpioSelector, (1<<selectorPin), GPIO_PIN_SET);
        }
    }

    return ret;
}

bool LS7366RDataSource::GetSignalMemoryBuffer(const uint32 signalIdx, const uint32 bufferIdx, void *&signalAddress) {
    bool ret = true;
    if (signalIdx == 0u) {
        signalAddress = &reset;
    } else if (signalIdx == 1u) {
        signalAddress = &counter;
    } else {
        ret = false;
    }
    return ret;
}

bool LS7366RDataSource::AllocateMemory(){
    return true;
}


bool LS7366RDataSource::Synchronise() {
    //first signal is reset
    if (gpioSelector != NULL) {
        HAL_GPIO_WritePin(gpioSelector, (1<<selectorPin), GPIO_PIN_RESET);
    }
    if(reset > 0u) {
        uint8 tx = (CLEAR | CNTR_REG);
        HAL_SPI_Transmit(spiHandle, &tx , 1, HAL_MAX_DELAY);
    }

    uint8 tx = READ | CNTR_REG;
    uint8 rx[4];

    HAL_SPI_Transmit(spiHandle, &tx , 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(spiHandle, rx , 4, HAL_MAX_DELAY);

    if(gpioSelector != NULL) {
        HAL_GPIO_WritePin(gpioSelector, (1<<selectorPin), GPIO_PIN_SET);
    }

    counter = (rx[0]<<24)|(rx[1]<<16)|(rx[2]<<8)|(rx[3]);
    if(invert) {
        counter *= -1;
    }

    return true;
}

const char8* LS7366RDataSource::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    if (direction == InputSignals) {
        return "MemoryMapSynchronisedInputBroker";
    } else {
        return "MemoryMapOutputBroker";
    }

    return "";
}

bool LS7366RDataSource::PrepareNextState(const MARTe::char8 *const currentStateName, const MARTe::char8 *const nextStateName) {
    counter = 0;
    if (gpioSelector != NULL) {
        HAL_GPIO_WritePin(gpioSelector, (1<<selectorPin), GPIO_PIN_RESET);
    }
    uint8 tx = (CLEAR | CNTR_REG);
    HAL_SPI_Transmit(spiHandle, &tx, 1, HAL_MAX_DELAY);
    if (gpioSelector != NULL) {
        HAL_GPIO_WritePin(gpioSelector, (1<<selectorPin), GPIO_PIN_SET);
    }

    return true;
}

CLASS_REGISTER(LS7366RDataSource, "1.0")
