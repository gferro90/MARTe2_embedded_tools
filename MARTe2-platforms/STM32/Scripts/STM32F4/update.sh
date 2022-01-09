#!/bin/sh

	grep -q _PATCHED main.c || sed -i '1s/^/extern void UserMainFunction(const void *arg);\r\#include "HandleDatabase.h"\r\n/' main.c
	grep -q _PATCHED main.c || sed -i '/int main(void)/,/while (1)/ s/StartDefaultTask/UserMainFunction/' main.c
	grep -q _PATCHED main.c || sed -i '/int main(void)/,/while (1)/ s/128/2048/' main.c
	grep -q _PATCHED main.c || sed -i 's/int main(void)/UART_HandleTypeDef *errorUartHandle=\&_UART_HANDLE_ERR;\nint main(void)/' main.c
	grep -q _PATCHED main.c || sed -i 's/osKernelStart();/HAL_TIM_Base_Start((TIM_HandleTypeDef*) GetHandle("htim6"));\n\r&/' main.c
	grep -q _PATCHED main.c || sed -i '/USER CODE END Includes/,/USER CODE BEGIN PV/ s/\(HandleTypeDef\) \(.*\).*;/\1 \2; REGISTER_HANDLE(\2, \&\2);/' main.c
	grep -q _PATCHED main.c || sed -i '1s/^/#define _PATCHED\r\n /' main.c
	grep -q 'CalibrateTimer' stm32f4xx_it.c || (sed -i 's/void NMI_Handler(void)/extern void CalibrateTimer();\n&/' stm32f4xx_it.c && sed -i 's/osSystickHandler();/&\nCalibrateTimer();/' stm32f4xx_it.c)
	if [ -f usbd_cdc_if.c ]; then cp usbd_cdc_if_template_c usbd_cdc_if.c; fi
	if [ -f usbd_cdc_if.h ]; then cp usbd_cdc_if_template_h usbd_cdc_if.h; fi	
	if [ ! -f usbd_cdc.h ]; then find . -name usbd_cdc.h | xargs -I found_file cp found_file . ; fi
	if [ ! -f usbd_cdc.c ]; then find . -name usbd_cdc.c | xargs -I found_file cp found_file . ; fi
	if [ ! -f port.c ]; then cp ./STM32_Config/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c . ; fi
	if [ ! -f heap_3.c ]; then cp ./STM32_Config/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_3.c . ; fi

