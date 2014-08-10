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
 * common utility
*/

#ifndef SMS_CONTROLLER_COMMON_H
#define SMS_CONTROLLER_COMMON_H

#define char_to_lower(c) ( 'A'<=c&&c<='Z' ? (c-'A'+'a') : (c) )
#define char_to_upper(c) ( 'a'<=c&&c<='z' ? (c-'a'+'A') : (c) )

#include <stdlib.h>

void *operator new(size_t objsize);
void operator delete(void* obj);
void delay_us(unsigned long us);
void delay_ms(unsigned long ms);

#endif
