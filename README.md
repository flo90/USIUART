# USIUART
This small library turns the USI (Universal Serial Interface) of an ATTiny microcontroller into an half-duplex UART receiver/transmitter.

## Usage
Just copy the folder USIUART into your project and add the c files to your build process. A Makefile is not provided by this lib.
Call usiuart_init() to initialize the library and enable gloabal interrupts with sei().
Use usiuart_getChar(...) to get the next character if available and usiuart_printStr(...) to transmit a string.
Note that trasmitting will interrupt receiving.

## Needed resources
The library occupies Timer0, PB0(RX) and PB1(TX).

## Test results
The library was tested on an ATTiny85 driven with 8MHz by the internal oscillator.
Even uncalibrated -measured ~8.2MHz- characteres were received and transmitted correctly.
Use a crystal to be safe.
