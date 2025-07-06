This configuration file prints ADCs on Logger. The cycle loop runs at 100 Hz frequency which is the same sampling frequency of the ADC.
The 4 values from the 4 ADC channels is printed every 10 cycles, therefore at a frequency of 10 Hz.
- Error handler on UART4 - Connect UART-USB RX on PC10 (UART4 - TX) and GND
- Connect PA3 (ch 3), PA6 (ch 6), PB1 (ch 9) and PC0 (ch 10) to the source to be measured (0-3.3V to 0-4095)
- See the ADC1 channels 3, 6, 9 and 10 printed at the configured frequency (5Hz)
 