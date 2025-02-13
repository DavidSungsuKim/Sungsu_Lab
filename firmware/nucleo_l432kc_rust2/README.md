# Tips
## How to use macros
   You can use these macros to see printouts in the command line.
      info!(""Hello World!={}", 1);
      debug!("debug info={}", 1);
      warn!(" ... ")
      error!(" ... ")


# Nucleo Setup
## Serial interface
   Module - USART2
	Pins (NucleoL432KC)
		### TX : A7 (PA2)
		### RX : A2 (PA3)
	Baudrate : 115200
	Line feed setting in Teraterm
		### TX : CR+LF
		### RX : CR
	When connecting a UART-to-USB module to the laptop, make sure the com port is not of Nucleo's.

## LED
   Pin - PB3