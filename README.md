# Arduino-STM32-software-UART-with-a-float-baudrate
This is a project of a fully software UART protocol implementation for low speed bauds where you can enter a float type value of baudrate (1.1 baud tested). It uses any digital OI pins as Rx and Tx. No interrupts used. 
## Why do you need such low baudrate's? 
I was facing a trouble when an old (1999 year) device starts communicating on 1.75 BAUDRATE and there were no software/hardware library or program I know to handle such speeds. So I decide to create my own and share its with world. It's probably not the performance efficient and ultra high skill coded but it works and now I can communicate with the device! And yes, at a baudrate = 1.75 bod, the byte send/receive took about 5 seconds :)
For now comunication is made by 2 functions:
* **uart_recv( data,  len,  bit_length);** 
  * **data** (uint8_t) is the byte array where you want to store incoming bytes; 
  * **len** (uint32_t) is a length of incoming message (here the timeout is neded so use len=1 for now); 
  * **bit_length** (uint32_t) is length of 1 bit in microseconds. You can enter a length you measure for example with oscilloscope (**uint32_t bit_time = 580000**), or enter baud you know by **#define baudRate 5.6** for example (the baud value will be converted to bit time automatically)
* **sendfromserial();** uses a USB-serial interface to send messages from PC terminal to Tx pin. The message you typed would be separated from x00 and leave only x0D at the end witch is 'Enter' comand for linux like systems. So if you send comand sudo in the adruino terminal the board would send s u d o + x0D masage to the Tx pin. (Useful when comunicating with old VT100 terminals)
## Hardware
I use a chep and powerfull STM32F103C [bluepill board](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html). It should work on any other STM32 board too. And maybe arduino Nano (not tested).
## Software
* [Standart Arduino IDE](https://www.arduino.cc/en/software)
* [STM32duino core](https://github.com/stm32duino/Arduino_Core_STM32)
## Limitations
* Max baudrate for RX and Tx lines is 38400 (tested with osciloscope and UART decoder). That's a lot especially when no interrupts used. And you can always use usual Serial librarys for higher bauds;
* Min baud is limited by timer overflow for functions micros() and delayMicroseconds(); (1.0 baud tested with signal analyzer). Probably lower bauds (<1) would work too.
* Only 8-N-1 (standard) mode implemented for now. (8-N-1 means 1 start bit, 8 data bits, 1 stop bit).
* Only half-duplex communication implemented (RS485-like).
While read timeout is not yet implemented you should connect both Rx and Tx pins to UART interface of the device because it will stuck on receive *while()* loop waiting for HIGH signal
## To do: 
* library; 
* read timeout; 
* duplex communication;
* interrupts.
* RTOS usage, maybe.
## Any help would be useful! 
## Thanks
Thanks to William Lai who implements c/c++ [software_uart](https://github.com/williamlai/software_uart) for Realtek RTL8195A board, and made [awesome detailed instruction](https://medium.com/@redmilk/implement-software-uart-from-2-gpio-1150e96c3d18) on how UART works.
