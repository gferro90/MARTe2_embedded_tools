In this example we initalise three timers:
TIM1 : period = 5s counter = 0-9999
TIM4 : period = 1s counter = 0-9999 to generate PWM signal on its channel 1 (pin D12 green LED)
TIM6 : period = 1ms counter = 0-999 provides us resolution to HighResolutionTimer

The GAM BufferGAM is synchronised on the TimerDataSource and reads the counter from TIM1 setting the PWM value to TIM4. The
result is the green LED blinking with different duty cycles with and the sequence has a period of 5s.

We use also the USBSignalDisplay to show with a desired frame rate the value of the signals.