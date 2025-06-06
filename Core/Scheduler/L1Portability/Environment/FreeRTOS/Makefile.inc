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
#############################################################

PACKAGE=Core/Scheduler/L1Portability
MARTe2_MAKEDEFAULT_DIR?=$(ROOT_DIR)/MakeDefaults
GENERIC_DIR=$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability/Environment

OBJSX=  EventSem.x \
    MutexSem.x \
    Processor.x \
    Sleep.x \
    ThreadInformation_Gen.x \
    Threads.x \
    ThreadsDatabase_Gen.x

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibDefs.$(TARGET)

INCLUDES+=-I$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/
INCLUDES+=-I$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability
INCLUDES+=-I$(ENVIRONMENT_BM_L1Portability_DIR)/$(ENVIRONMENT)

all: $(OBJS)    $(CPPS) \
    $(BUILD_DIR)/L1Portability$(LIBEXT)

include depends.$(TARGET)

include $(MARTe2_MAKEDEFAULT_DIR)/MakeStdLibRules.$(TARGET)

