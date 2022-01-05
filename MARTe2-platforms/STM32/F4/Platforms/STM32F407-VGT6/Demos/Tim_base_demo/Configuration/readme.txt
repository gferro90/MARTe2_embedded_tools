In this example we initalise three timers:
TIM1 : period = 1ms counter = 0-999
TIM4 : period = 1ms counter = 0-999
TIM6 : period = 1ms counter = 0-999 provides us resolution to HighResolutionTimer

The GAM BufferGAM is synchronised on the TimerDataSource and reads the counter from TIM1 and TIM2 setting
the result in USBSignalDisplay to show with a desired frame rate the values.