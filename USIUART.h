/*  Copyright (C) 2016  Florian Menne

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef USIUART_H_
#define USIUART_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * Initialize USIUART
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

#endif /* USIUART_H_ */
