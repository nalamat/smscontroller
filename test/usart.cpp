/*************************************************************************
 *                                                                       *
 * This file is a part of the SMSController project:                     *
 * Remotely control home appliances via text messages                    *
 * Copyright (C) 2011-2014, Nima Alamatsaz, All rights reserved          *
 * Email: nnalamat@gmail.com                                             *
 * Web:   http://github.com/nalamat/smscontroller                        *
 *                                                                       *
 * SMSController is free software: you can redistribute the software     *
 * and/or modify it under the terms of the GNU General Public License    *
 * version 3 as published by the Free Software Foundation.               *
 *                                                                       *
 * SMSController is distributed in the hope that it will be useful,      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with SMSController. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                       *
 *************************************************************************/

#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "usart.h"

void usart_init()
{
	// on xtal=8mhz
	// 9600 baud rate
	UCSRC = (1<<URSEL | 1<<UCSZ0 | 1<<UCSZ1);
	UCSRB = (1<<TXEN  | 1<<RXEN );
	UCSRA = 0;
	UBRRL = 0x33;
	UBRRH = 0x00;
}

void usart_send_char(const char ch)
{
	while ( !(UCSRA & 1<<UDRE) );
	UDR = ch;
}

void usart_send_str(const char *str)
{
	while ( *str ) usart_send_char(*(str++));
}

