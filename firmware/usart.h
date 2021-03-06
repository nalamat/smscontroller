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

/*
 * vasete ertebate serial ba micro,
 * ghabl az estefade bayad tanha yek bar ba
 * farakhandane usart_init() rah andazi shavad,
 * ersale dade tavassote usart_send_str() anjam mishavad,
 * dade haye daryafti dar interrupte RXC dar yek queue
 * push mishavand ta loope asliye barname hengame niyaz
 * anha ra bekhanad
*/

#ifndef SMS_CONTROLLER_USART_H
#define SMS_CONTROLLER_USART_H

#include "queue.h"

extern queue_str usart_recv_buf;

void usart_init();
void usart_send_char(const char ch);
/*
 * sends each character of the given string
 * in order, plus an optional ending character:
 * end=true		=> cr (\r 13)
 * end=false	=> ctrl+z (26)
*/
void usart_send_str(const char *str);
/*
 * waits until a character is received
 * note: this function is blocking
*/
char usart_recv_char();
/*
 * sets a callback for when data arrives
 * data is stored in usart_recv_buf
 * without leading and ending \r\n
 * note: it doesn't block the run
*/
void usart_recv_set(void (*call)());

#endif
