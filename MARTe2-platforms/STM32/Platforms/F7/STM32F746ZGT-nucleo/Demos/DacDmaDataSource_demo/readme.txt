This configuration file sends an incremental counter (0 to 4095) to the two DAC channels, then reads from ADC1 channels 9 and 10 and prints the 
two values to the Logger (UART4). The user is supposed to connect the two DAC channels to the two ADC channels in order to read the incremental counter.
The cycle loop executes at 100 Hz frequency. Same frequency is set to the DAC and the ADC1 trigger timer. The Logger prints every 10 cycles, therefore 
at a frequency of 10 Hz.
- Error handler on UART4 - Connect UART-USB RX on PC10 (UART4 - TX) and GND
- Connect DAC channel 1 (PA4) to ADC1 channel 9 (PB1) 
- Connect DAC channel 2 (PA5) to ADC1 channel 10 (PC0)
- See the incremental counter sent to the DAC and ADC1 channels 9 and 10 printed at 10 Hz

 