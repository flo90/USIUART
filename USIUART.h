/*
 * usi_uart.h
 *
 *  Created on: 01.12.2016
 *      Author: flo
 */

#ifndef SRC_USIUART_H_
#define SRC_USIUART_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Initialize the usi uart
 */
void usiuart_init(void);

/**
 * Gets next character
 * @param dst Pointer to the destination
 * @return False = No character, True = Next Character received
 */
bool usiuart_getChar(char *dst);

/**
 * Transmit a string.
 * Interrupts every ongoing RX.
 * @param string Null terminated string
 * @return True = Success, False = currently Sending
 */
bool usiuart_printStr(char* string);

#endif /* SRC_USIUART_H_ */
