/*************************************************************************
 *                                                                       *
 * This file is a part of the SMSController project:                     *
 * Remotely control home appliances via text messages                    *
 * Copyright (C) 2011-2014 Nima Alamatsaz, All rights reserved           *
 * Email: nialamat@gmail.com                                             *
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
#include "queue.h"

queue_str usart_recv_buf;

void usart_init()
{
	// on xtal=8mhz
	// 9600 baud rate
	UCSRC = (1<<URSEL | 1<<UCSZ0 | 1<<UCSZ1);
	UCSRB = (1<<TXEN  | 1<<RXEN  | 1<<RXCIE);
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

char usart_recv_char()
{
	while ( !(UCSRA & 1<<RXC) );
	return UDR;
}

ISR (USART_RXC_vect)
{
	char c=UDR, l=0;
	
	/////////////////////////////////////
	//usart_recv_buf_len = 1;
	//usart_recv_buf[0] = c;
	//usart_recv_buf[1] = 0;
	//if (usart_recv_call) usart_recv_call();
	//return;
	/////////////////////////////////////

	int i = 0;
	char *str = (char*)malloc(200);

	// receive characters and store them
	// in usart_recv_buf until \r\n is received
	// note: ignoring first and second char: \r\n
	do
	{
		if ( i==0 && (c=='\r'||c=='\n') ) continue;
		//if ( c=='\r' ) break;
		if ( c=='\n' && l=='\r' )
		{
			i--;
			break;
		}

		str[i] = c;
		l = c;
		++i;
	}
	while ( c=usart_recv_char(), 1 );

	str[i] = 0;
	usart_recv_buf.push(str);
}
