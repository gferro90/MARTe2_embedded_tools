#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
#  and the Development of Fusion Energy ('Fusion for Energy')
#
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent  
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#  
# http://ec.europa.eu/idabc/eupl
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
# $Id: Makefile.inc 3 2012-01-15 16:26:07Z aneto $
#
#############################################################
export PROJECT_NAME=Usb_demo
export MARTe2_DIR=/home/pc/MARTe2Project/GIT/MARTe2
export MARTe2_Components_DIR=/home/pc/MARTe2Project/GIT/MARTe2-components
export TARGET=arm_cort_m7_stm
export PLATFORM_FAMILY1=STM32
export PLATFORM_FAMILY2=F7
export PLATFORM_NAME=STM32F746ZGT-nucleo
export DRIVER_PLATFORM=STM32F7xx
export SPEC_DEFS= -DSTM32F746xx -DSTM32746ZG_Nucleo -DUSE_FREERTOS -D_HAL_H="stm32f7xx_hal.h" -D__TIMER__NAME__=TIM6 -D_UART_HANDLE_ERR=huart4
#-DconfigCHECK_FOR_STACK_OVERFLOW=1 -DINCLUDE_uxTaskGetStackHighWaterMark=1 -D_UPLOAD_CFG 
export SCRIPT_DIR=$(PWD)/STM32/Scripts
export CODE_DIR=$(PWD)/STM32
export PLATFORM_FAMILY=$(PLATFORM_FAMILY1)/Platforms/$(PLATFORM_FAMILY2)
PLATFORM_DIR=$(PLATFORM_FAMILY)/$(PLATFORM_NAME)
export PROJECT_DIR=${CURDIR}/$(PLATFORM_DIR)/Demos/$(PROJECT_NAME)
PROJECT_CONF_INCLUDES=
#CFG_FILE=$(SCRIPT_DIR)/CfgUploader.cfg
CFG_FILE=$(PROJECT_DIR)/Configuration/UsbCommunication.cfg
export SCHEDULER_DIR=$(CODE_DIR)/Scheduler
SCHEDULER_NAME = BasicScheduler


MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

C_FILES = $(wildcard *.c)
S_FILES = $(wildcard *.s)

LINKER_LD_FILE=STM32F746ZGTx_FLASH.ld 

OBJSX := $(C_FILES:.c=.x)
OBJSX += $(S_FILES:.s=.x)

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

BUILD_DIR=Build

INCLUDES += -I.
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/STM32F7xx_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/

INCLUDES += -I/home/pc/STM32Cube/Repository/STM32Cube_FW_F7_V1.4.0/Drivers/BSP/STM32F7xx_Nucleo_144/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/Common/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/otm8009a/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Src/


all: 	$(OBJS) \
	$(BUILD_DIR)/output$(HEXEXT)\
	$(BUILD_DIR)/output$(EXEEXT)\
	$(BUILD_DIR)/output$(ELFEXT)
	
include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)
	