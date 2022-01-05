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


OBJSX=	ErrorFunction.x


SPB = $(PROJECT_NAME)_app_bsp.x\
	$(PROJECT_NAME)_app/Debug.x\
	../../DataSources/UDPCommunication.x\
	$(CODE_DIR)/DataSources/GenericStreamDriver.x\
	$(CODE_DIR)/GAMs/BufferGAM.x $(CODE_DIR)/GAMs/SumGAM.x\
	../../Scheduler.x\
	../../DataSources/Timer.x\
	../../Streams/UdpStream.x\
	$(CODE_DIR)/CfgUploader.x

MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults
-include PlatformDefinitions

LINKER_LD_FILE=$(PROJECT_DIR)/$(PROJECT_NAME)_app/src/lscript.ld


include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

ADD_C_FILES = $(shell ls $(PROJECT_DIR)/$(PROJECT_NAME)_app/src/*.c | xargs -n 1 basename)


BUILD_DIR=Build



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
INCLUDES += $(PROJECT_CONF_INCLUDES)

LIBRARIES_STATIC += ../../Build/UDPCommunication.a
LIBRARIES_STATIC += $(CODE_DIR)/Build/GenericStreamDriver.a
LIBRARIES_STATIC += $(CODE_DIR)/Build/BufferGAM.a
LIBRARIES_STATIC += $(CODE_DIR)/Build/SumGAM.a
LIBRARIES_STATIC += ../../Build/BasicScheduler.a
LIBRARIES_STATIC += ../../Build/TimerDataSource.a
LIBRARIES_STATIC += $(MARTe2_DIR)/Build/$(TARGET)/Core/BareMetal/MARTeCoreB.a
LIBRARIES_STATIC +=../../Build/UdpStream.a
LIBRARIES_STATIC += $(CODE_DIR)/Build/CfgUploader.a


OBJS += $(BUILD_DIR)/startup_main.o
OBJS += $(ADD_C_FILES:%.c=$(BUILD_DIR)/%.o)

all: update_main $(SUBPROJ) update_objs $(OBJS) \
	$(BUILD_DIR)/output$(HEXEXT)\
	$(BUILD_DIR)/output$(ELFEXT)

update_main:
	cp $(PROJECT_DIR)/$(PROJECT_NAME)_app/Debug/makefile $(PROJECT_DIR)/$(PROJECT_NAME)_app/Debug/Makefile.gcc
	cp $(PROJECT_DIR)/$(PROJECT_NAME)_app_bsp/Makefile $(PROJECT_DIR)/$(PROJECT_NAME)_app_bsp/Makefile.gcc

update_objs:
	cp $(PROJECT_DIR)/$(PROJECT_NAME)_app/Debug/src/*.o $(BUILD_DIR)
	cp $(PROJECT_DIR)/$(PROJECT_NAME)_app/src/Xilinx.spec $(PROJECT_DIR)/


include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

