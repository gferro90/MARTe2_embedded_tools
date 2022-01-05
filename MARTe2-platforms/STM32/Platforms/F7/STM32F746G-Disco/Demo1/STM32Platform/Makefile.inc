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

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

C_FILES = $(wildcard *.c)

OBJSX := $(C_FILES:.c=.x)

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

BUILD_DIR=../Build

INCLUDES += -I.
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/STM32F7xx_HAL_Driver/Inc/
INCLUDES += -I$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Device/ST/STM32F7xx/Include/

all: update_main \
	$(OBJS) \
	$(BUILD_DIR)/STM32PlatformLib$(LIBEXT) \
	copy_startup

update_main:
	grep -q 'UserMainFunction' main.c || sed -i '1s/^/extern void UserMainFunction(const void *arg);\r\n /' main.c
	grep -q 'osThreadCreate' main.c || sed -i 's/osKernelStart();/osThreadDef(UserMainThread, UserMainFunction, osPriorityNormal, 0, configMINIMAL_STACK_SIZE); osThreadCreate(osThread(UserMainThread), NULL);&/' main.c
	
copy_startup:
	cp startup*.s ../

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

checkdefs:
ifndef STM_CUBE_FW_DIR
	$(error Please export in the variable STM_CUBE_FW_DIR the location of the STM Cube FW.)
endif
ifndef MARTe2_DIR
	$(error Please export in the variable MARTe2_DIR the location of folder where MARTe2 is installed.)
endif
ifndef TARGET
	$(error Please export in the variable TARGET the target platform (e.g. armv7em-freertos.)
endif


