#/bin/sh

export STM_CUBE_FW_DIR=/home/pc/STM32Cube/Repository/STM32Cube_FW_F4_V1.13.0/
export MARTe2_DIR=/home/pc/MARTe2Project/GIT/MARTe2-STM-develop
export TARGET=armv7em-freertos
make -f Makefile.gcc $1



