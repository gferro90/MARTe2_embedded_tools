#!/bin/sh

export MARTe2_DIR=/home/pc/MARTe2Project/GIT/MARTe2
export MARTe2_Components_DIR=/home/pc/MARTe2Project/GIT/MARTe2-components
export ROOT_DIR=/home/pc/MARTe2Project/GIT/MARTe2_embedded_tools
export MAKEDEF_DIR=$ROOT_DIR
export ARCHITECTURE_BM_L0Types_DIR=$ROOT_DIR/Core/BareMetal/L0Types/Architecture
export ARCHITECTURE_BM_L1Portability_DIR=$ROOT_DIR/Core/BareMetal/L1Portability/Architecture
export ENVIRONMENT_BM_L1Portability_DIR=$ROOT_DIR/Core/BareMetal/L1Portability/Environment
export ENVIRONMENT_FS_L1Portability_DIR=$ROOT_DIR/Core/FileSystem/L1Portability/Environment
export ENVIRONMENT_SC_L1Portability_DIR=$ROOT_DIR/Core/Scheduler/L1Portability/Environment
export LD_LIBRARY_PATH=/home/pc/MARTe2Project/GIT/MARTe2/Build/x86-linux/Core/
export OUTPUT_DIR=Build
export TARGET=arm_cort_m7_stm

