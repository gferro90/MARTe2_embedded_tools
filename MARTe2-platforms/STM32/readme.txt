1. Run the STM32CubeMX
2. Configure STM32 for your project and save the project (e.g. ~/Projects/STM32Dump/STM32F746G-Disco-Demo1/)
3. Create an STM32Project folder in your project (e.g. F7/Platforms/STM32F746G-Disco/Demo1/STM32Platform/)
4. Run python PATH_TO_Scripts/STM32ExtractFiles.py PATH_TO_STM32CubeMX_PROJECT_FOLDER STM32CubeMX_PROJECT_NAME)
4.1 e.g. ../../../../../Scripts/STM32ExtractFiles.py ~/Projects/STM32Dump/STM32F746G-Disco-Demo1/ STM32F746G-Disco-Demo1 .
5. export the following variables
5.1 export TARGET=armv7em-freertos
5.2 export MARTe2_DIR=PATH_TO_MARTe2_DIR (e.g. ~/Projects/MARTe2-dev/)
5.3 export STM_CUBE_FW_DIR=PATH_TO_STM32CUBE_FW_DIR (e.g. ~/Projects/STM32Cube_FW_F7_V1.4.0/)
5.4 export FREE_RTOS_CONFIG_DIRECTORY=PATH_TO_LOCATION_OF_FREE_RTOS_CONFIG (e.g. ~/MARTe2-platforms/STM32/F7/Platforms/STM32F746G-Disco/Demo1/STM32Platform/)
