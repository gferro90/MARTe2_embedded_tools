## GpioReaderDataSource.cfg
This configuration file reads from the GPIOF pins which are configured as GPIO inputs in STM32CubeMX. The GPIOF mask is written to the Logger.
- Error handler on UART4 - Connect UART-USB RX on PC10 (UART4 - TX) and GND
- Cable one pin of GPIOF (PF0 to PF15) to GND or 3.3V and see the value changing accordingly on the Logger.

## GpioWriterDataSource.cfg
This configuration file toggles the pins connected to the user LEDs. LED1 is assigned to PB1, LED2 to PB7 and LED3 to PB14.
- Error handler on UART4 - Connect UART-USB RX on PC10 (UART4 - TX) and GND
- Initial value is LED1 and LED3 ON (GPIOB value 16385: (1<<14)+(1<<1))
- The value is inverted and masked with (1<<14)+(1<<7)+(1<<1) to toggle the pins.