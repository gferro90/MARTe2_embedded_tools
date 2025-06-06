/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_cdc_if.c
 * @version        : v1.0_Cube
 * @brief          : Usb device for Virtual Com Port.
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

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"
#include "stm32f7xx_hal.h"

extern USBD_HandleTypeDef hUsbDeviceFS;
volatile uint8_t USB_initialised = 0;
extern USBD_DescriptorsTypeDef FS_Desc;
extern uint8_t *UserRxBufferFS;
extern uint8_t *UserTxBufferFS;
extern UART_HandleTypeDef *errorUartHandle;

struct {
    uint8_t *Buffer;
    int Position;
    int Last;
    char IsReadData;
    USBD_CDC_HandleTypeDef *Handle;
} s_RxBuffer;

static int8_t CDC_Init_FS_Custom(void) {
    s_RxBuffer.Buffer = UserRxBufferFS;
    memset(UserRxBufferFS, 0, APP_RX_DATA_SIZE);
    /* USER CODE BEGIN 3 */
    /* Set Application Buffers */
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
    USB_initialised = 1;
    return (USBD_OK);
    /* USER CODE END 3 */
}

static int8_t CDC_Receive_FS_Custom(uint8_t *Buf, uint32_t *Len) {
    /* USER CODE BEGIN 6 */

    s_RxBuffer.Position = 0;
    s_RxBuffer.Last += *Len;
    s_RxBuffer.Last %= APP_RX_DATA_SIZE;
    s_RxBuffer.IsReadData = 1;
//  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
//  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return (USBD_OK);
    /* USER CODE END 6 */
}

int USBInitialized() {
    return USB_initialised;
}

USBD_HandleTypeDef* USBGetHandle() {
    return &hUsbDeviceFS;
}

void USBOpen() {
    // USB CDC initialization
    // CDC means comunication mode
    /* Init Device Library,Add Supported Class and Start the library*/
    USBD_Interface_fops_FS.Init = CDC_Init_FS_Custom;
    USBD_Interface_fops_FS.Receive = CDC_Receive_FS_Custom;

    USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

    USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);

    USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);

    USBD_Start(&hUsbDeviceFS);

    while (!USB_initialised) {
        HAL_Delay(1000);
    }
}

void USBClose() {
    // stop the device
    USBD_Stop(&hUsbDeviceFS);

    // de-initialisation
    USBD_DeInit(&hUsbDeviceFS);

}

int USBWrite(const char *const txBuffer, uint32_t *sizeIn, int8_t nonBlock) {

    uint32_t size = *sizeIn;
    // High speed communication
    // if the size is greater than the packet size
    if (size > CDC_DATA_HS_OUT_PACKET_SIZE) {
        uint32_t offset;

        // divide the buffer in packets and send them separately
        for (offset = 0; offset < size; offset += CDC_DATA_HS_OUT_PACKET_SIZE) {
            uint32_t todo = (uint32_t)(MIN(CDC_DATA_HS_OUT_PACKET_SIZE, (int ) (size - offset)));
            // go recursively
            int ret = USBWrite(((char*) txBuffer) + offset, &todo, nonBlock);
            if (!ret) {
                size = offset + todo;
                *sizeIn = size;
                return 0;
            }
        }
        *sizeIn = size;
        return 1;
    }

    // wait for previous transfer
    USBD_CDC_HandleTypeDef *pCDC = (USBD_CDC_HandleTypeDef*) hUsbDeviceFS.pClassData;
    while (pCDC->TxState) {
    }

    // transmit the packet
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, (uint8_t*) txBuffer, size);
    if (USBD_CDC_TransmitPacket(&hUsbDeviceFS) != USBD_OK) {
        *sizeIn = size;
        return 0;
    }

    // wait until transfer is done if blocking
    if (!nonBlock) {
        while (pCDC->TxState) {
        }
    }
    *sizeIn = size;
    return 1;

}

static int USBPrivateRead(char *const rxBuffer, uint32_t size) {

    // the remained received size to be copied
    int remaining = (s_RxBuffer.Last >= s_RxBuffer.Position) ? (s_RxBuffer.Last - s_RxBuffer.Position) : ((APP_RX_DATA_SIZE - s_RxBuffer.Position) + s_RxBuffer.Last);

    // take the minimum
    int todo = MIN(remaining, (int ) size);

    if (todo <= 0) {
        USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS + s_RxBuffer.Last);
        USBD_CDC_ReceivePacket(&hUsbDeviceFS);
        return 0;
    }

    // copy data to the output buffer.
    memcpy(rxBuffer, s_RxBuffer.Buffer + s_RxBuffer.Position, todo);
    s_RxBuffer.Position += todo;
    s_RxBuffer.Position %= APP_RX_DATA_SIZE;

    return todo;
}

int USBRead(char *const rxBuffer, uint32_t *sizeIn, int8_t nonBlock) {

    uint32_t size = *sizeIn;
    if (nonBlock) {
        size = USBPrivateRead(rxBuffer, size);
    } else {
        uint32_t cont = 0;
        // reads all the packets until size is consumed
        while (cont < size) {
            cont += USBPrivateRead(rxBuffer + cont, size - cont);
        }
    }
    *sizeIn = size;
    return 1;

}

int USB_RXBufferSize() {
    return (s_RxBuffer.Last >= s_RxBuffer.Position) ? (s_RxBuffer.Last - s_RxBuffer.Position) : ((APP_RX_DATA_SIZE - s_RxBuffer.Position) + s_RxBuffer.Last);
}

int USB_RXBufferPosition() {
    return s_RxBuffer.Position;
}

void USBSync() {
    memset(UserRxBufferFS, 0, APP_RX_DATA_SIZE);
    s_RxBuffer.Position = 0;
    s_RxBuffer.Last = 0;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
 * @}
 */

/**
 * @}
 */

