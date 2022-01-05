#/bin/sh

#export STM_CUBE_FW_DIR=/home/pc/STM32Cube/Repository/STM32Cube_FW_F4_V1.13.0/
export TARGET=zynq7010
make -f Makefile.gcc $1



