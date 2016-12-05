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

void usiuart_init(void);
uint8_t usiuart_getChar(void);

bool usiuart_printStr(char* string);

#endif /* SRC_USIUART_H_ */
