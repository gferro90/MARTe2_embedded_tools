# User Manual

### Preparation

Download **MARTe2** and switch to *develop* branch which is usually the last stable release.

```shell
$ git clone https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2.git
$ cd MARTe2
$ git checkout develop
```

Comment out the not used library to produce a lighter compiled binary to be flashed and fit onto the embedded board.

```shell
$ vim Source/Core/BareMetal/Makefile.inc

...
LIBRARIES_STATIC=$(BUILD_DIR)/L0Types/L0TypesB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L1Portability/L1PortabilityB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L2Objects/L2ObjectsB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L3Streams/L3StreamsB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4Configuration/L4ConfigurationB$(LIBEXT)
#LIBRARIES_STATIC+=$(BUILD_DIR)/L4Logger/L4LoggerB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4Messages/L4MessagesB$(LIBEXT)
#LIBRARIES_STATIC+=$(BUILD_DIR)/L4HttpService/L4HttpServiceB$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L5GAMs/L5GAMsB$(LIBEXT)
#LIBRARIES_STATIC+=$(BUILD_DIR)/L6App/L6AppB$(LIBEXT)
...

$ vim Source/Core/BareMetal/L4Configuration/Makefile.inc

...
# Remove RunTimeEvaluator.x and RunTimeEvaluatorFunction.x which is very heavy
OBJSX=	AnyObject.x \
		AnyTypeCreator.x \
		ConfigurationDatabase.x\
		ConfigurationDatabaseNode.x\
		ConfigurationParserI.x \
		FloatToInteger.x \
		IntegerToFloat.x \
		IntrospectionStructure.x \
		JsonParser.x \
		LexicalAnalyzer.x \
		StringToFloat.x \
		StringToInteger.x \
		TypeConversion.x \
		TokenInfo.x \
		Token.x \
		ParserI.x \
		StandardParser.x \
		ValidateBasicType.x \
		XMLParser.x
...		

$ vim Source/Core/Scheduler/Makefile.inc

...
LIBRARIES_STATIC+=$(BUILD_DIR)/L1Portability/L1PortabilityS$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L3Services/L3ServicesS$(LIBEXT)
#LIBRARIES_STATIC+=$(BUILD_DIR)/L4LoggerService/L4LoggerServiceS$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L4Messages/L4MessagesS$(LIBEXT)
#LIBRARIES_STATIC+=$(BUILD_DIR)/L4StateMachine/L4StateMachineS$(LIBEXT)
LIBRARIES_STATIC+=$(BUILD_DIR)/L5GAMs/L5GAMsS$(LIBEXT)
...

```

Change also in MARTe2/Source/Core/Scheduler/L3Services/MultiClientEmbeddedThread.cpp 

```shell
 information.SetThreadNumber(static_cast<uint32>(threadId));
```
to 

```shell
information.SetThreadNumber((uint32)(threadId));
```

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
$ cd STM32F4_EmbeddedProject/MARTe
$ vim PlatformDefinitions.sh

export MARTe2_DIR=/home/giuseppe/MARTe2Project/GIT/MARTe2
export MARTe2_Embedded_Tools_DIR_BASE=/home/giuseppe/MARTe2Project/GIT/MARTe2_embedded_tools
export MARTe2_Embedded_Tools_DIR=$MARTe2_Embedded_Tools_DIR_BASE/MARTe2-platforms/STM32
export MARTe2_Components_DIR=/home/pc/MARTe2Project/GIT/MARTe2-components
export TARGET=arm_cort_m4_stm
export SPEC_DEFS='-DSTM32F407xx -DSTM32F4_Discovery -DUSE_FREERTOS -D_HAL_H="stm32f4xx_hal.h" -D__TIMER__NAME__=TIM6 -D__TIMER__NAME__=TIM6 -D_UART_HANDLE_ERR=huart2 -DERROR_ON_USB'
export SCRIPT_DIR=$MARTe2_Embedded_Tools_DIR/Scripts/STM32F4
export SCHEDULER_DIR=$MARTe2_Embedded_Tools_DIR/Scheduler
export PROJECT_NAME=STM32_Config
export PROJECT_DIR=$PWD
export PROJECT_CONF_INCLUDES=$PWD/ToolConfiguration
export DRIVER_PLATFORM=STM32F4xx
export LINKER_LD_FILE=$PWD/stm32_flash.ld
cp $PROJECT_CONF_INCLUDES/$PROJECT_NAME/Inc/FreeRTOSConfig.h $PROJECT_CONF_INCLUDES
cp $PROJECT_CONF_INCLUDES/$PROJECT_NAME/Inc/mxconstants.h $PROJECT_CONF_INCLUDES
export FREE_RTOS_CONFIG_DIRECTORY=$PROJECT_CONF_INCLUDES

$ source PlatformDefinitions.sh
```

In the MARTe_embedded_tools folder, change the paths accordingly to your environment

```shell
$ cd MARTe_embedded_tools
$ vim Core/Makefile.arm_cort_m4_stm

...
export MARTe2_DIR=/home/giuseppe/MARTe2Project/GIT/MARTe2
export MARTe2_Embedded_Tools_DIR_BASE=/home/giuseppe/MARTe2Project/GIT/MARTe2_embedded_tools
export MARTe2_MAKEDEFAULT_DIR=$(MARTe2_Embedded_Tools_DIR_BASE)/MakeDefaults
export MARTe2_Components_DIR=/home/giuseppe/MARTe2Project/GIT/MARTe2-components
export TARGET=arm_cort_m4_stm
...

$ vim MakeDefaults/MakeStdLibDefs.arm_cort_m4_stm

...
STM_CUBE_FW_DIR=/home/giuseppe/STM32Cube/Repository/STM32Cube_FW_F4_V1.13.0
FREE_RTOS_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/FreeRTOS
PORT_MACRO_H_DIRECTORY=$(STM_CUBE_FW_DIR)/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
CMSIS_DIRECTORY=$(STM_CUBE_FW_DIR)/Drivers/CMSIS/Include/
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

Compile the MARTe2 libraries for STM32F4

```shell
$ cd Core
$ make -f Makefile.arm_cort_m4_stm
```

### Build the project 

Compile the project

```shell
$ cd STM32F4_EmbeddedProject/MARTe
$ make -f Makefile.arm_cort_m4_stm
```

load the binary on the STM board

```shell
$ cd Build
$ ./loader.sh
```

### Video Tutorial 
https://raw.githubusercontent.com/gferro90/MARTe2_embedded_tools/main/Documentation/Tutorial.webm







