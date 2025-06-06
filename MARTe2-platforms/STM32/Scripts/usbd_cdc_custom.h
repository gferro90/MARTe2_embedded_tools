/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.h
  * @version        : v1.0_Cube
  * @brief          : Header for usbd_cdc_if.c file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CDC_IF_H__
#define __USBD_CDC_IF_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc.h"
int USBInitialized();

USBD_HandleTypeDef *USBGetHandle();
void USBOpen();
void USBClose();
int USBWrite(const char* const txBuffer, uint32_t *sizeIn, int8_t nonBlock);
int USBRead(char* const rxBuffer, uint32_t *sizeIn, int8_t nonBlock);
int USB_RXBufferSize();
int USB_RXBufferPosition();
void USBSync();


#ifdef __cplusplus
}
#endif

#endif /* __USBD_CDC_IF_H__ */

