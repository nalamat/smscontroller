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
#include <util/delay.h>
#include "common.h"

void *operator new(size_t objsize)
{
	return malloc(objsize);
}

void operator delete(void* obj)
{
	free(obj);
}

void delay_us(unsigned long us)
{
	unsigned long ms = us / 1000;
	us %= 1000;
	while (ms--) _delay_ms(1);
	while (us--) _delay_us(1);
}

void delay_ms(unsigned long ms)
{
	while (ms--) _delay_ms(1);
}
