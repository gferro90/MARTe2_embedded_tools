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

OBJSX=EfficientDriver.x\
	EfficientReader.x\
	EfficientWriter.x\
	FifoIO.x\
	NiConfigurator.x\
	NiDevice.x\
	NiDeviceT.x\
	EmbeddedPackagerReader.x\
	EmbeddedPackagerSyncReader.x\
	CircularFIFOAcq.x


MAKEDEFAULTDIR=$(MARTe2_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

BUILD_DIR=../../Build

INCLUDES += -I$(MARTe2_Embedded_Tools_DIR)/Scheduler
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L2Objects/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L3Streams/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Configuration/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L4Messages/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/BareMetal/L5GAMs/
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L3Services
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4LoggerService
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4Messages
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L4StateMachine
INCLUDES += -I$(MARTe2_DIR)/Source/Core/Scheduler/L5GAMs
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L3Streams
INCLUDES += -I$(MARTe2_DIR)/Source/Core/FileSystem/L4LoggerService
INCLUDES += -I$(MARTE2_COMPONENTS)/Source/Components/DataSources/CircularBufferAcqDriver
INCLUDES += -I$(LEVEL_0)
INCLUDES += $(PROJ_INC)


CFLAGS += -D_LABVIEW_GEN_H=$(_LABVIEW_GEN_H)


all: $(OBJS) $(SUBPROJ) \
	$(BUILD_DIR)/FifoIO$(LIBEXT)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

