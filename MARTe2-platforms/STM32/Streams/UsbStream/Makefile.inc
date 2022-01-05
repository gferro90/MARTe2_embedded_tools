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
OBJSX=UsbStream.x

SPB =

MAKEDEFAULTDIR=$(MARTe2_Embedded_Tools_DIR_BASE)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)


INCLUDES += -I.
INCLUDES += -I$(PROJECT_CONF_INCLUDES)
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/Common/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/BSP/Components/otm8009a/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Src/
INCLUDES += -I$(STM_DIR)/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/
INCLUDES += -I$(STM_DIR)/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/
INCLUDES += -I$(STM_DIR)/Middlewares/Third_Party/FreeRTOS/Source/include/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/Environment/FreeRTOS/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/$(DRIVER_PLATFORM)_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/$(DRIVER_PLATFORM)/Include/
INCLUDES += -I../StreamParent
INCLUDES += -I$(ENVIRONMENT_BM_L1Portability_DIR)/$(ENVIRONMENT)


all: $(OBJS) $(SUBPROJ) \
	$(BUILD_DIR)/UsbStream$(LIBEXT)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

