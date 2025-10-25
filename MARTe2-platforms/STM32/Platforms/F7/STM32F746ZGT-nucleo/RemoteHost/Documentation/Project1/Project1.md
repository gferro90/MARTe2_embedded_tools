# Project 1

### PCB Description

![Alt text](./Pictures/PCB.jpeg "PCB Layout")

As in the figure above we have 4 ADC inputs. The input is normally 0-3.3V when jumper is on the right but it can be set to 0-12V shifting the jumper on the right.
Putting the jumper on the right, the input passes through a partitor of 1k - 2.7k resistors (divided by 3.7)
The ADC have been configured to acquire at a max rate of 10 kHz.
We have also 2 DAC outputs. Same as for the ADC, when jumper is on the left the output is in the range 0-3.3V, while if the jumper is on the right the range is 0-12V.
Putting the jumper on the right, the output passes through an op-amp direct configuration amplifier with 1k and 2.7k resistors (amplified by 3.7). The used op-amp is LM358 OpAmp (a dual op-amp chip). 
Tha DAC have been configured to generate at a max rate of 10 kHz.

![Alt text](./Pictures/LM358.png "LM358")

The UART4 used for debug has been connected to a serial to USB converter in order to be easily connected to a common computer to receive the debug log messages.
The PF0, PF1, PF2, PF3 outputs have been routed to optical outputs. The selected optical transmitter has been the VL HFBR-1521Z.
The PE0, PE2, PE3, PE4 inputs have been routed to optical inputs. The selected optical receiver has been and HFBR-2521Z.

![Alt text](./Pictures/OpticalDigVL.png "LM358")

As specified in the datasheet, the double nand port chip SN75451B has been selected for the transmitter circuit.

![Alt text](./Pictures/SN75451B.png "SN75451B")
![Alt text](./Pictures/SN75451B_Pinout.png "SN75451B")

### Project1 MARTe2 application on STM32F746ZFGT-nucleo
The MARTe2 application running on the STM32F746ZFGT-nucleo cyclically executes two GAMs

* IO: IOGAM synchronizing on Timer (systick) at 100 Hz. It reads the inputs coming on UDP for the generation of the sine waves on the two DAC channels (frequency, offset, amplitude and phase) on the DDB. It reads only from UDP the digital outputs to be set and it sets directly on GPIO. It reads the digital inputs from GPIO and the 4 values from the ADC channels writing them directly to UDP for the remote host.
* Sine: SineGeneratorGAM that takes in input the parameters frequency, phase, offset and amplitude and generates the two sine waveforms, one on each DAC channel.

Concerning the data sources, ADC and DAC are also configured to acquire and generate at 100 Hz, one sample each cycle loop.

### Project1 MARTe2 application on Remote Host
On the remote host, the application is separated in two threads.

The read thread is executing the following GAMs:

* ReadUDP: IOGAM to read the 4 ADC channels values and the digital inputs from UDP from the PCB. The signals are written on DDB.
* ExpandDigitalOut: ExtractBitGAM which expands the uint32 value from UDP containing the digital inputs in the bit mask in the 16 uint8 signals one for each digital input. This signal are monitored by the HTTP HttpDataMonitor called HttpMonitor_Digital, so they are visible on the MARTe2 HTTP interface.
* WriteToMqtt: IOGAM to write the 4 ADC channels values to MosquittoDataSource. The topic is "ADC" and can be configured. The mosquitto broker is running separately and integrated to grafana allowing to see the live streaming of the acquired samples in real-time from ADC.

The write thread is executing the following GAMs:

* Sine0_Params: a ConstantGAM whose signals are set from the HttpMessageInterface HttpMessageInterface_Sine0 and then from the user on the HTTP MARTe2 interface. The signals are the frequency, phase, offset and amplitude of the sine wave to be generated on the DAC channel 0.
* Sine1_Params: a ConstantGAM whose signals are set from the HttpMessageInterface HttpMessageInterface_Sine1 and then from the user on the HTTP MARTe2 interface. The signals are the frequency, phase, offset and amplitude of the sine wave to be generated on the DAC channel 1.
* GPIO_Outputs: a ConstantGAM whose signals are set from the HttpMessageInterface HttpMessageInterface_DigOut and then from the user on the HTTP MARTe2 interface. There is a 0-1 uint8 signal for each digital output.
* Sync: IOGAM which synchronises the thread on time at 10 Hz
* CompactDigitalIn: CompactBitGAM to compact the 16 uint8 digital output signal in a single uint16 signal.
* WriteUDP: IOGAM to write the two sines parameters and the digital outputs to UDP.

On the remote host the application can be much more flexible as the resources are not that limited as in the STM32 board. We can also add the InfluxDataSource to archive the analog and digital signals and plot them to grafana which is very well integrated with influx.

Influx: user=home, psw=grafana

Mosquitto: user=grafana, psw=grafana90

Grafana: user=admin, pws=****09

### How To
Connect the RJ45 cable for UDP, a USB cable to the USB OTG port of the STM32 and another USB cable to power-up the board.

Optionally, connect another USB cable to the UART4-USB converter to see error logs (use minicom at 115200 on the device)

Compile and load the STM32F746ZFGT-nucleo binary as explained in the [README.md](../../../../../../../../README.md).

Load the Project1 cfg file on the STM32 board.

```shell
$ cd MARTe2_embedded_tools/MARTe2-platforms/STM32/Scripts/
$ ./SendConfiguration.ex ../Platforms/F7/STM32F746ZGT-nucleo/Demos/Project1_demo/Project1.cfg
```
Run the Project1 application on the host.

```shell
$ cd MARTe2_embedded_tools/MARTe2-platforms/STM32/Platforms/F7/STM32F746ZGT-nucleo/RemoteHost
$ ./Startup.sh Project1.cfg
```








