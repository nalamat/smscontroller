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

#include <math.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

void timer_init()
{
	// Timer1: 0.25*64*62500 = 1000000us
	// OCR1A CTC mode, clk/64 prescaler
	TCNT1 = 0;
	OCR1A = 62500-1;
	TCCR1A = 0;
	TCCR1B = (1<<WGM12) | 0b011;
	
	TIMSK = 1<<OCIE1A;
}

int main()
{
	DDRD  = 0b10000000;
	PORTD = 0b00000000;

	timer_init();
	usart_init();
	sei();

	_delay_ms(3000);
	usart_send_str("\r\nCall Ready\r\n");
	_delay_ms(3000);
	usart_send_str("\r\n+CMTI: \"SM\",1\r\n");
	_delay_ms(3000);
	usart_send_str("\r\n+CMGR: \"REC UNREAD\",\"+989132082986\",,\"10/12/14,22:41:09+14\"\r\n");
	_delay_ms(3000);
	usart_send_str("AB 10101\r\n");

	while (1);

	return 0;
}

int counter = 0;

ISR (TIMER1_COMPA_vect)
{
	PORTD ^= 0b10000000;
}
