# USIUART
This small library turns the USI (Universal Serial Interface) of an ATTiny microcontroller into an half-duplex UART receiver/transmitter.

## Usage
Just copy the folder USIUART into your project and add all c files to your build process. A Makefile is not provided by this library.
Call usiuart_init() to initialize the library and enable global interrupts with sei().
Use usiuart_getChar(...) to get the next character if available and usiuart_printStr(...) to transmit a string.
Note that trasmitting will interrupt receiving.

## Configuration
The library is configurable by #define-directives. The following definitions are available:
F_CPU               : CPU Frequency - Normally already defined by build enviornment.
USE_BITSWAP_TABLE   : 0 -> Calculate, 1 -> Use table.
TIMER_DIVIDER       : Sets divider of Timer0. 1, 8, 64, 256 and 1024 are available.
BAUD                : Baudrate.
RXBUFFERSIZE        : Receive buffer size. Only numbers like 2, 4, 8, 16, 32 ... are allowed.
To easily update this lib from github without setting the configuration again, it's recommended to pass the definitions by the compiler flag -D.

## Possible errors
Depending on the values of F_CPU, TIMER_DIVIDER and BAUD it's possible that the calculated timer periode overflows the compare register.
This error is catched by an preprocessor macro. If this error occures, it's possible to reduce the value by increasing DIVIDER_TIMER and/or BAUD - Start with increasing DIVIDER_TIMER if possible.
Last and mostly useless option is to reduce the clock frequency (F_CPU).

## Needed resources
The library occupies Timer0, PB0(RX) and PB1(TX).

## Test results
The library was tested on an ATTiny85 driven with 8MHz by the internal oscillator.
Even uncalibrated -measured ~8.2MHz- characteres were received and transmitted correctly.
Use a crystal if you need to be safe.
