In this example we initalise the timer:
TIM6 : period = 1ms counter = 0-999 provides us resolution to HighResolutionTimer

and the UART4 

There are two UARTCommunication data sources, one to read an external value that will be written by the BufferGAM to
another UARTCommunication data source. The sync point is the read signal from UART and the frequency is set to zero 
because the system acts as a slave of a master which has to provide data to be read from UART port (TX=A0 RX=A1).
