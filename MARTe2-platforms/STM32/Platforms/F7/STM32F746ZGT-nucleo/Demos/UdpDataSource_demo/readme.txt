** UdpReceiverDataSource.cfg **
To be used together with UdpReceiverDataSource.cfg on remote host
Simple data source to receive from UDP and log

** UdpSenderDataSource.cfg **
Simple data source to send to UDP

** AdcDacUdp.cfg **
To be used together with AdcDacUdp.cfg on remote host
10 Hz frequency. Same frequency is set to the DAC and the ADC1 trigger timer. 
- Error handler on UART4 - Connect UART-USB RX on PC10 (UART4 - TX) and GND
- Connect DAC channel 1 (PA4) to ADC1 channel 9 (PB1) 
- Connect DAC channel 2 (PA5) to ADC1 channel 10 (PC0)
- The incremental counter is sent to the DAC and ADC1 channels 9 and 10 
- Counter and ADC channels are sent over UDP

** AdcDacUdpSine.cfg **
To be used together with AdcDacUdpSine.cfg on remote host
10 Hz frequency. Same frequency is set to the DAC and the ADC1 trigger timer. 
- Error handler on UART4 - Connect UART-USB RX on PC10 (UART4 - TX) and GND
- Connect DAC channel 1 (PA4) to ADC1 channel 9 (PB1) 
- Receive sine parameters from remote host (amplitude, phase, frequency, offset) 
- Generates sine on DAC and acquire from ADC
- Send the ADC to UDP