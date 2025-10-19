# User Manual

### Preparation

Download **MARTe2_embedded_tools**

```shell
$ git clone https://github.com/gferro90/MARTe2_embedded_tools.git
```

Download the embedded project template **STM32F4_EmbeddedProject**

```shell
$ git clone https://github.com/gferro90/STM32F4_EmbeddedProject.git
```

Download **STM32CubeMX** from [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) 
Download **st-link** (to flash the binary) from [st-link](https://www.st.com/en/development-tools/stsw-link004.html)


### Project code generation

Open STM32CubeMX and load the template project in *STM32F4_EmbeddedProject/MARTe/ToolConfiguration/STM32_Config/STM32Config.ioc*
STM32CubeMX will ask if to migrate the project to the last version or if to use the old version of the drivers. Choose to use the old version (it should be V1.13.0) and STM32CubeMX will automatically download the drivers.

```shell
$ cd STM32CubeMX/
$ ./STM32CubeMX/
```

If necessary, make changes to the configuration and then generate the code.

### Build MARTe2 

In the template project folder, check and eventually change paths and variables accordingly to the environment;

```shell
$ cd STM32F7_EmbeddedProject/MARTe
$ vim PlatformDefinitions.sh

#!/bin/sh

#Exports needed by the project
export MARTe2_Embedded_Tools_DIR_BASE=/home/giuseppe/MARTe2Project/GIT/MARTe2_embedded_tools
export MARTe2_DIR=$MARTe2_Embedded_Tools_DIR_BASE/MARTe2
export MARTe2_Embedded_Tools_DIR=$MARTe2_Embedded_Tools_DIR_BASE/MARTe2-platforms/STM32
export MARTe2_Components_DIR=/home/giuseppe/MARTe2Project/GIT/MARTe2-components
export MAKEDEFAULTDIR=$MARTe2_Embedded_Tools_DIR_BASE/MakeDefaults
export TARGET=arm_cort_m7_stm
export SPEC_DEFS='-DSTM32F746xx -DSTM32746ZG_Nucleo -DUSE_FREERTOS -D_HAL_H="stm32f7xx_hal.h" -D__TIMER__NAME__=TIM6 -D_TIMER_HANDLE=htim6 -D_UART_HANDLE_ERR=huart4 -D_UPLOAD_CFG -DLWIP_UDP=1'
#-DERROR_ON_USB 
export SCRIPT_DIR=$MARTe2_Embedded_Tools_DIR/Scripts
export SCHEDULER_DIR=$MARTe2_Embedded_Tools_DIR/Scheduler
export PROJECT_NAME=STM32_Config
export PROJECT_DIR=$PWD
export PROJECT_CONF_INCLUDES=$PWD/ToolConfiguration
export DRIVER_PLATFORM=STM32F7xx
export LINKER_LD_FILE=$PWD/stm32_flash.ld
cp $PROJECT_CONF_INCLUDES/$PROJECT_NAME/Core/Inc/FreeRTOSConfig.h $PROJECT_CONF_INCLUDES
cp $PROJECT_CONF_INCLUDES/$PROJECT_NAME/Core/Inc/mxconstants.h $PROJECT_CONF_INCLUDES
export FREE_RTOS_CONFIG_DIRECTORY=$PROJECT_CONF_INCLUDES

$ source PlatformDefinitions.sh
```

In the MARTe_embedded_tools folder, change the paths accordingly to your environment

```shell
$ cd MARTe_embedded_tools
$ vim Core/Makefile.arm_cort_m7_stm

...
export MARTe2_DIR=/home/giuseppe/MARTe2Project/GIT/MARTe2_embedded_tools/MARTe2
export MARTe2_Embedded_Tools_DIR_BASE=/home/giuseppe/MARTe2Project/GIT/MARTe2_embedded_tools
export MARTe2_MAKEDEFAULT_DIR=$(MARTe2_Embedded_Tools_DIR_BASE)/MakeDefaults
export MARTe2_Components_DIR=/home/giuseppe/MARTe2Project/GIT/MARTe2-components
export TARGET=arm_cort_m7_stm
...

$ vim MakeDefaults/MakeStdLibDefs.arm_cort_m7_stm

...
STM_CUBE_FW_DIR=/home/giuseppe/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.2
FREE_RTOS_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/FreeRTOS/Source/include
FREE_RTOS_DIRECTORY_CMSIS=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
PORT_MACRO_H_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/
CMSIS_DIRECTORY=$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
USB_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
LWIP_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/LwIP/src/include
LWIP_ARCH_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/LwIP/system/
FREE_RTOS_CONFIG_DIRECTORY?=$(MARTe2_Embedded_Tools_DIR_BASE)/Core/BareMetal/L1Portability/Environment/FreeRTOS

export ARCHITECTURE_BM_L0Types_DIR=$(MARTe2_DIR)/Source/Core/BareMetal/L0Types/Architecture
export ARCHITECTURE_BM_L1Portability_DIR=$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/Architecture
export ENVIRONMENT_BM_L1Portability_DIR=$(MARTe2_DIR)/Source/Core/BareMetal/L1Portability/Environment
export ENVIRONMENT_BM_L6App_DIR=$(MARTe2_DIR)/Source/Core/BareMetal/L6App/Environment
export ENVIRONMENT_FS_L1Portability_DIR=$(MARTe2_DIR)/Source/Core/FileSystem/L1Portability/Environment
export ENVIRONMENT_SC_L1Portability_DIR=$(MARTe2_DIR)/Source/Core/Scheduler/L1Portability/Environment
export MARTe2_PORTABLE_ENV_DIR=$(MARTe2_Embedded_Tools_DIR_BASE)/Core
export MARTe2_PORTABLE_ARCH_DIR=$(MARTe2_Embedded_Tools_DIR_BASE)/Core
...

```

Compile the MARTe2 libraries for STM32F7

```shell
$ cd Core
$ make -f Makefile.arm_cort_m7_stm
```

### Build the project 

Compile the project

```shell
$ cd STM32F7_EmbeddedProject/MARTe
$ make -f Makefile.arm_cort_m7_stm
```

load the binary on the STM board

```shell
$ cd Build
$ ./loader.sh
```

### Video Tutorial 
https://raw.githubusercontent.com/gferro90/MARTe2_embedded_tools/main/Documentation/Tutorial.webm







