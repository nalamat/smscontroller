/*************************************************************************
 *                                                                       *
 * This file is a part of the SMSController project:                     *
 * Remotely control home appliances via text messages                    *
 * Copyright (C) 2011-2014, Nima Alamatsaz, All rights reserved          *
 * Email: nnalamat@gmail.com                                             *
 * Web:   http://github.com/nalamat/smscontroller                        *
 *                                                                       *
 * SMSController is free software: you can redistribute it and/or modify *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * any later version.                                                    *
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

#include "queue.h"
#include "common.h"

link_str::link_str()
{
	data = 0;
	sub = 0;
}

link_str::~link_str()
{
	if (data) delete data;
	if (sub) delete sub;
}

queue_str::queue_str()
{
	begin = 0;
}

queue_str::~queue_str()
{
	if (begin) delete begin;
}

char *queue_str::top()
{
	if ( begin ) return begin->data;
	else return 0;
}

void queue_str::pop()
{
	link_str *temp = begin;
	begin = temp->sub;
	temp->sub = 0;
	delete temp;
}

void queue_str::push(char *data)
{
	link_str *add = new link_str();
	add->data = data;

	if ( !begin ) begin = add;
	else
	{
		link_str *seek = begin;
		while ( seek->sub ) seek = seek->sub;
		seek->sub = add;
	}
}

bool queue_str::empty()
{
	return begin==0;
}


///////////////////////////////////////////////


link_int::link_int()
{
	data = 0;
	sub = 0;
}

link_int::~link_int()
{
	if (data) delete data;
	if (sub) delete sub;
}

queue_int::queue_int()
{
	begin = 0;
}

queue_int::~queue_int()
{
	if (begin) delete begin;
}

int *queue_int::top()
{
	if ( begin ) return begin->data;
	else return 0;
}

void queue_int::pop()
{
	link_int *temp = begin;
	begin = temp->sub;
	temp->sub = 0;
	delete temp;
}

void queue_int::push(int *data)
{
	link_int *add = new link_int();
	add->data = data;

	if ( !begin ) begin = add;
	else
	{
		link_int *seek = begin;
		while ( seek->sub ) seek = seek->sub;
		seek->sub = add;
	}
}

bool queue_int::empty()
{
	return begin==0;
}
