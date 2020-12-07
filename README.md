# Arduino-STM32-software-UART-with-a-float-baudrate
This is a project of a fully software UART protocol implementation for low speed bauds where you can enter a float type value of baudrate (1.1 baud tested). It uses any digital OI pins as Rx and Tx. No interrupts used. 
## Why do you need such low baudrate's? 
I was facing a trouble when an old (1999 year) device starts communicating on 1.75 BAUDRATE and there were no software/hardware library or program I know to handle such speeds. So I decide to create my own and share its with world. It's probably not the performance efficient and ultra high skill coded but it works and now I can communicate with the device! And yes, at a baudrate = 1.75 bod, the byte send/receive took about 5 seconds :)
## Limitations
Max baudrate is 38400. That's a lot especially when no interrupts used. An you can always use usual Serial librarys for higher bauds (tested with osciloscope); Min baud is limited by timer overflow for functions micros() and delayMicroseconds(); (1.0 baud tested with signal analyzer). Probably lower bauds (<1) would work too. Only 8-N-1 (standard) mode implemented for now. Only half-duplex communication implemented (RS485-like).
## To do: 
* library; 
* read timeouts; 
* duplex communication;
* interrupts.
