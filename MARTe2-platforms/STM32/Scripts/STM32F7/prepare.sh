#!/bin/sh
	ls $PROJECT_DIR/ToolConfiguration/Makefile* || cp $SCRIPT_DIR/Makefile.* $PROJECT_DIR/ToolConfiguration
	ls $PROJECT_DIR/ToolConfiguration/usbd_cdc_if_template_* || cp $SCRIPT_DIR/usbd_cdc_if_template_* $PROJECT_DIR/ToolConfiguration
	ls $PROJECT_DIR/ToolConfiguration/STM32ExtractFiles.py || cp $SCRIPT_DIR/STM32ExtractFiles.py $PROJECT_DIR/ToolConfiguration
	ls stm32f7xx.h || cd $PROJECT_DIR/ToolConfiguration; python3 STM32ExtractFiles.py $PROJECT_NAME $PROJECT_NAME . ; cd -
	ls $PROJECT_DIR/ToolConfiguration/HandleDatabase.* || cp $SCRIPT_DIR/HandleDatabase.* $PROJECT_DIR/ToolConfiguration
	ls $PROJECT_DIR/ToolConfiguration/patch.sh || cp $SCRIPT_DIR/STM32F7/patch.sh $PROJECT_DIR/ToolConfiguration
	if [ -f $PROJECT_DIR/ToolConfiguration/usbd_cdc_if.c ]; then cp $PROJECT_DIR/ToolConfiguration/usbd_cdc_if_template_c $PROJECT_DIR/ToolConfiguration/usbd_cdc_if.c; fi
	if [ -f $PROJECT_DIR/ToolConfiguration/usbd_cdc_if.h ]; then cp $PROJECT_DIR/ToolConfiguration/usbd_cdc_if_template_h $PROJECT_DIR/ToolConfiguration/usbd_cdc_if.h; fi	
	if [ ! -f $PROJECT_DIR/ToolConfiguration/usbd_cdc.h ]; then find . -name usbd_cdc.h | xargs -I found_file cp found_file $PROJECT_DIR/ToolConfiguration/ ; fi
	if [ ! -f $PROJECT_DIR/ToolConfiguration/usbd_cdc.c ]; then find . -name usbd_cdc.c | xargs -I found_file cp found_file $PROJECT_DIR/ToolConfiguration/ ; fi
	if [ ! -f $PROJECT_DIR/ToolConfiguration/port.c ]; then cp $PROJECT_DIR/ToolConfiguration/$PROJECT_NAME/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c $PROJECT_DIR/ToolConfiguration/ ; fi
	if [ ! -f $PROJECT_DIR/ToolConfiguration/heap_3.c ]; then cp $PROJECT_DIR/ToolConfiguration/$PROJECT_NAME/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_3.c $PROJECT_DIR/ToolConfiguration/ ; fi
#	if [ ! -f $PROJECT_DIR/ToolConfiguration/usbd_core.c ]; then find . -name usbd_core.c | xargs -I found_file cp found_file $PROJECT_DIR/ToolConfiguration/ ; fi
#	if [ ! -f $PROJECT_DIR/ToolConfiguration/usbd_ctlreq.c ]; then find . -name usbd_ctlreq.c | xargs -I found_file cp found_file $PROJECT_DIR/ToolConfiguration/ ; fi
#	if [ ! -f $PROJECT_DIR/ToolConfiguration/usbd_ioreq.c ]; then find . -name usbd_ioreq.c | xargs -I found_file cp found_file $PROJECT_DIR/ToolConfiguration/ ; fi
