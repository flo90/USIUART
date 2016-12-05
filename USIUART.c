/*
 * usi_usart.c
 *
 *  Created on: 01.12.2016
 *      Author: flo
 */


#include "USIUART.h"

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/iotn85.h>
#include <avr/interrupt.h>

#include "bitSwap.h"

#define F_TIMER 1000000
#define BAUD 9600
#define TIMER_TICK ((float) F_TIMER/BAUD)

#define RXBUFFERSIZE 8

enum
{
	RX = 0,
	TX,
	NEXTCHAR,
};

static uint8_t rxBuffer[RXBUFFERSIZE];
static volatile uint8_t rxWriter = 0;
static uint8_t rxReader = RXBUFFERSIZE-1;
static volatile uint8_t state;
static volatile uint8_t tempTxChar;
static volatile char* currentChar;

void usiuart_init()
{
	state = RX;

	//PB0 as input
	DDRB &= ~_BV(PB0);

	//PB1 as output and high
	DDRB |= _BV(PB1);
	PORTB |= _BV(PB1);

	/*
	 * Set Timer settings
	 */
	TCCR0A |= _BV(WGM01);
	TCCR0B |= _BV(CS01);

	/*
	 * Enable PCINT0
	 */
	GIFR = _BV(PCIF);
	PCMSK |= _BV(PCINT0);
	GIMSK |= _BV(PCIE);

	OCR0A = (uint8_t) TIMER_TICK + TIMER_TICK/2;
}

uint8_t usiuart_getChar()
{
	uint8_t temp;
	uint8_t data;

	//Calculate next reader position
	temp = (rxReader + 1) & (RXBUFFERSIZE-1);

	//Return if no char available
	if(temp == rxWriter) return 0;

	//Get data from buffer and bitswap
	data = bitSwap(rxBuffer[temp]);

	rxReader = temp;

	return data;
}


bool usiuart_printStr(char* string)
{
	/*
	 * If RX is currently in progress don't care - bad luck...
	 */

	if(state != RX) return false;

	//Disable PCINT0 interrupt
	PCMSK = 0;

	//Disable TIM Interrupt
	TIMSK = 0;

	//Disable USI
	USICR = 0;

	state = TX;


	currentChar = string;

	tempTxChar = bitSwap(*currentChar++);

	//Prefill with data and startbit
	USIDR = tempTxChar>>1;

	TCNT0 = 0;
	OCR0A = (uint8_t) TIMER_TICK;

	/*
	 * USI settings
	 */
	USISR = _BV(USIOIF) | 14;
	USICR = _BV(USICS0) | _BV(USIOIE) | _BV(USIWM0);

	return true;
}

ISR(PCINT0_vect)
{
	//Only react if Pin is low (startbit)
	if( PINB & _BV(PB0) ) return;
	PCMSK = 0;

	TCNT0 = 0;
	OCR0A = (uint8_t) TIMER_TICK + TIMER_TICK/2;

	// Clear flag
	TIFR = _BV(OCF0A);

	//Enable OCIE0A
	TIMSK = _BV(OCIE0A);


	/*
	 * USI settings
	 */
	USISR = _BV(USIOIF) | 8;
	USICR |= _BV(USICS0) | _BV(USIOIE);

}

ISR(TIMER0_COMPA_vect)
{
	OCR0A = (uint8_t) TIMER_TICK;
	TIMSK = 0;
}

ISR(USI_OVF_vect)
{
	uint8_t temp;
	uint8_t data;

	/*
	 * Handle RX
	 */
	if(state == RX)
	{
		USICR = 0;
		data = USIDR;
		//Calculate next position
		temp = (rxWriter + 1) & (RXBUFFERSIZE-1);

		//Check if next position collides with reader
		if(temp != rxReader)
		{
			//Write to rxBuffer
			rxBuffer[rxWriter] = data;
		}

		//Set rxWriter to next position
		rxWriter = temp;

		PCMSK = _BV(PCINT0);
	}

	/*
	 * Handle TX
	 */
	else if(state == TX)
	{
		USIDR = (tempTxChar<<1) | _BV(0);

		USISR = _BV(USIOIF) | 8;
		USICR = _BV(USICS0) | _BV(USIOIE) | _BV(USIWM0);
		state = NEXTCHAR;
	}

	/*
	 * Handle NEXTCHAR
	 */
	else if (state == NEXTCHAR)
	{
		/*
		 * Check for next character
		 */
		if(*currentChar)
		{
			//Get next character
			tempTxChar = bitSwap(*currentChar++);

			//Prefill with data and startbit
			USIDR = tempTxChar>>1;

			TCNT0 = 0;
			state = TX;

			/*
			 * USI settings
			 */
			USISR = _BV(USIOIF) | 14;
			return;
		}

		/*
		 * Reenable RX
		 */

		//Disable USI
		USICR = 0;

		//Change to RX state
		state = RX;

		//Clear Pin change interrupt flag
		GIFR |= _BV(PCIF);

		//Reenable interrupt on PCINT0
		PCMSK = _BV(PCINT0);
	}
}
