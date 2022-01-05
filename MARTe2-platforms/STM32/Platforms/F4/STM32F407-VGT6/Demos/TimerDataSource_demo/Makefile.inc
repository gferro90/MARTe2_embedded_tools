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


OBJSX =	startup_stm32f407xx.x

SPB = ToolConfiguration.x\
	../../../../../DataSources/TimerDataSource.x\
	../../../../../CfgUploader.x\
	../../../../../Streams/UsbStream.x\
	../../../../../Scheduler.x\
	$(MARTe2_Components_DIR)/Source/Components/GAMs/IOGAM.x\
	$(MARTe2_Components_DIR)/Source/Components/DataSources/LoggerDataSource.x




MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults
-include PlatformDefinitions

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

BUILD_DIR=Build

LINKER_LD_FILE=../stm32_flash.ld

INCLUDES += -IToolConfiguration
INCLUDES += -I../Scheduler
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/$(DRIVER_PLATFORM)/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/Common/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/otm8009a/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Src/
INCLUDES += -I$(SCHEDULER_DIR)

#use dynamic linking!
LIBRARIES_STATIC = Build/ToolConfiguration.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/DataSources/TimerDataSource/TimerDataSource.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/CfgUploader/CfgUploader.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/UsbStream/UsbStream.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/Components/DataSources/LoggerDataSource/LoggerDataSource.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/Components/GAMs/IOGAM/IOGAM.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/Scheduler/Scheduler/BasicScheduler.a
LIBRARIES_STATIC += $(ROOT_DIR)/Build/$(TARGET)/Core/MARTe2.a


OBJS += $(BUILD_DIR)/startup_main.o

all: update $(SUBPROJ) $(OBJS) \
	$(BUILD_DIR)/output$(HEXEXT)\
	$(BUILD_DIR)/output$(EXEEXT)\
	$(BUILD_DIR)/output$(ELFEXT)

update:
	ls $(PROJECT_DIR)/ToolConfiguration/usbd_cdc_if_template_* || cp $(SCRIPT_DIR)/usbd_cdc_if_template_* $(PROJECT_DIR)/ToolConfiguration
	ls $(PROJECT_DIR)/ToolConfiguration/STM32ExtractFiles.py || cp $(SCRIPT_DIR)/STM32ExtractFiles.py $(PROJECT_DIR)/ToolConfiguration
	ls $(PROJECT_DIR)/ToolConfiguration/HandleDatabase.* || cp $(SCRIPT_DIR)/HandleDatabase.* $(PROJECT_DIR)/ToolConfiguration
	ls $(PROJECT_DIR)/ToolConfiguration/update.sh || cp $(SCRIPT_DIR)/update.sh $(PROJECT_DIR)/ToolConfiguration






include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

