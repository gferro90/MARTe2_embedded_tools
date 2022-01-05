#!/bin/sh

	grep -q _PATCHED main.c || sed -i '1s/^/extern void UserMainFunction(const void *arg);\r\n /' main.c
	grep -q _PATCHED main.c || sed -i '/int main(void)/,/while (1)/ s/StartDefaultTask/UserMainFunction/' main.c
	grep -q _PATCHED main.c || sed -i 's/osKernelStart();/osThreadDef(UserMainThread, UserMainFunction, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);\n\rosThreadCreate(osThread(UserMainThread), NULL);\n\r&/' main.c
	ls Addons.hdd && grep -q _PATCHED main.c || sed -i 's/int main(void)/#include "HandleDatabase.h"\r\n&/' main.c
	grep -q _PATCHED main.c || sed -i '/USER CODE END Includes/,/USER CODE BEGIN PV/ s/\(HandleTypeDef\) \(.*\).*;/\1 \2; REGISTER_HANDLE("\2", \&\2);/' main.c
	sed -i '1s/^/#define _PATCHED\r\n /' main.c
	
    	


