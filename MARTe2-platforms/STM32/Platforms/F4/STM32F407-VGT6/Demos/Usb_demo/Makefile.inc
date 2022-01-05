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


OBJSX=	startup_stm32f407xx.x \
	ErrorFunction.x
#startup_main.x

SPB = STM32Platform.x ../../DataSources/USBCommunication.x ../../DataSources/GenericStreamDriver.x ../../DataSources/USBSignalDisplay.x ../../GAMs/BufferGAM.x ../../GAMs/SumGAM.x ../../Scheduler.x ../../DataSources/Timer.x

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults
-include PlatformDefinitions

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

BUILD_DIR=Build

LINKER_LD_FILE=../stm32_flash.ld

INCLUDES += -ISTM32Platform
INCLUDES += -I../../Scheduler
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/Common/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/STM32F4xx_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/otm8009a/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/STM32F4xx_HAL_Driver/Src/


LIBRARIES_STATIC = Build/STM32PlatformLib.a
LIBRARIES_STATIC += ../../Build/USBCommunication.a
LIBRARIES_STATIC += ../../Build/GenericStreamDriver.a
LIBRARIES_STATIC += ../../Build/USBSignalDisplay.a
LIBRARIES_STATIC += ../../Build/BufferGAM.a
LIBRARIES_STATIC += ../../Build/SumGAM.a
LIBRARIES_STATIC += ../../Build/BasicScheduler.a
LIBRARIES_STATIC += ../../Build/TimerDataSource.a
LIBRARIES_STATIC += $(MARTe2_DIR)/Build/$(TARGET)/Core/BareMetal/MARTeCoreB.a
LIBRARIES_STATIC += $(MARTe2_DIR)/Build/$(TARGET)/Core/Scheduler/MARTeCoreS.a

#CPPFLAGS += -Dmain=UserMainFunction 
#CPPFLAGS += -DconfigCHECK_FOR_STACK_OVERFLOW=1 
#CPPFLAGS += -DINCLUDE_uxTaskGetStackHighWaterMark=1

#CPPFLAGS += -D__CFG__FILE__=Configuration/UsbSignalDisplay.cfg
#CPPFLAGS += -D__CFG__FILE__=Configuration/UsbCommunication.cfg

OBJS += $(BUILD_DIR)/startup_main.o
#OBJS += $(shell find $(MARTe2_DIR)/Build/$(TARGET)/Core/BareMetal/ -name *.o | grep -v Test)
#OBJS += $(shell find $(MARTe2_DIR)/Build/$(TARGET)/Core/Scheduler/ -name *.o | grep -v Test)

all: $(SUBPROJ) $(OBJS) \
	$(BUILD_DIR)/output$(HEXEXT)\
	$(BUILD_DIR)/output$(EXEEXT)\
	$(BUILD_DIR)/output$(ELFEXT)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

