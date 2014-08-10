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

#include "at.h"
#include "usart.h"
#include "common.h"

bool at_ready = false;

enum at_rsp_enum
{
	AT_OK,
	AT_RDY,
	AT_CALL_READY,
	AT_CPIN_READY,
	AT_CPIN_NOT_INSERTED,
	AT_CMTI,				// sms received
	AT_CMGR,				// sms read
	AT_POWER_LOW_WARNING,
	AT_POWER_LOW_DOWN,
	AT_NORMAL_POWER_DOWN,
};

int at_rsp_len = 10;
const char *at_rsp_str[] = 
{
	"ok",
	"rdy",
	"call ready",
	"+cpin: ready",
	"+cpin: not inserted",

	"+cmti: ",
	"+cmgr: ",

	"power low warning",
	"power low down",
	"normal power down",
};

void (*at_rsp_ready_call)();
void (*at_rsp_sms_sent_call)(int index);
void (*at_rsp_sms_read_call)(const char *str, const char *num);

void at_init()
{
}

void at_parse(const char *str)
{
	// 'rsp_last' is used for certain responses that have more
	// than one section, e.g. sms read: a header and a body
	static int rsp_last=0;
	int rsp=0;

	if ( rsp_last ) rsp = rsp_last;
	else for ( int i=0 ; i<at_rsp_len && !rsp ; ++i )
	{
		for ( int j=0 ; ; ++j )
		{
			if ( at_rsp_str[i][j]==0 )
			{
				rsp = i;
				break;
			}

			if ( str[j]==0 ) break;
			if ( at_rsp_str[i][j]!=char_to_lower(str[j]) ) break;
		}
	}

	switch (rsp)
	{
		case AT_CALL_READY:
			at_ready = true;
			if ( at_rsp_ready_call ) at_rsp_ready_call();
			break;
		case AT_CPIN_NOT_INSERTED:
			at_ready = false;
			break;
		case AT_CMTI:
			at_rsp_sms_sent_parse(str);
			break;
		case AT_CMGR:
			if ( rsp_last )
			{
				at_rsp_sms_read_parse(str, false);
				rsp_last = 0;
			}
			else
			{
				at_rsp_sms_read_parse(str, true);
				rsp_last = rsp;
			}
			break;
		case AT_POWER_LOW_DOWN:
			at_ready = false;
			break;
		case AT_NORMAL_POWER_DOWN:
			at_ready = false;
			break;
	}
}

void at_rsp_ready_set( void (*call)() )
{
	at_rsp_ready_call = call;
}

void at_rsp_sms_sent_set( void (*call)(int index) )
{
	at_rsp_sms_sent_call = call;
}

void at_rsp_sms_sent_parse( const char* str )
{
	int i=0;
	while ( str[i++]!=',' );

	int index = 0;
	while ( str[i] ) index = index*10 + (int)(str[i++]-'0');

	if ( at_rsp_sms_sent_call ) at_rsp_sms_sent_call(index);
}

void at_rsp_sms_read_set( void (*call)(const char *str, const char *num) )
{
	at_rsp_sms_read_call = call;
}


void at_rsp_sms_read_parse( const char* str, bool header )
{
	static char num_last[15] = "";

	if ( header )
	{

		int j = 0;
		bool comma=false, quote=false;
		int str_len = -1;
		while (str[++str_len]);

		for ( int i=0 ; i<str_len ; ++i )
		{
			if ( comma && quote )
			{
				if ( str[i]=='"' ) break;
				num_last[j++] = str[i];
			}
			else if ( comma )
			{
				if ( str[i]=='"' ) quote=true;
			}
			else
			{
				if ( str[i]==',' ) comma=true;
			}
		}

		num_last[j] = 0;
	}
	else
	{
		int i=0;
		//char c, l=0;

		//while ( (c=str[i++])!='\r' );
		// this is useful when str contains both
		// header and the body of response
		//while ( !((c=str[i++])=='\n' && l=='\r') ) l=c;

		if ( at_rsp_sms_read_call ) at_rsp_sms_read_call(str+i, num_last);
	}
}

void at_cmd_sms_format()
{
	usart_send_str("at+cmgf=1");
	usart_send_char('\r');
}

void at_cmd_sms_send(const char *str, const char *num)
{
	usart_send_str( "at+cmgs=\"" );
	usart_send_str( num );
	usart_send_str( "\"" );
	usart_send_char( '\r' );
	usart_send_str( str );
	usart_send_char( (char)26 );
	//usart_send_char( '\r' );
}

void at_cmd_sms_read(int index)
{
	if ( index<1 || 99<index  ) return;
	usart_send_str( "at+cmgr=" );
	if ( index>=10 )
	usart_send_char( (char)(index/10)+'0' );
	usart_send_char( (char)(index%10)+'0' );
	usart_send_char( '\r' );
}

void at_cmd_sms_delete(int index)
{
	if ( index<1 || 99<index  ) return;
	usart_send_str( "at+cmgd=" );
	if ( index>=10 )
	usart_send_char( (char)(index/10)+'0' );
	usart_send_char( (char)(index%10)+'0' );
	usart_send_char( '\r' );
}

void at_cmd_at()
{
	usart_send_str( "at" );
	usart_send_char( '\r' );
}

/*
T:
	at+cpowd=1
	at+cmgf=1			select sms message format (always use)
	at+cmgs=			send sms message
	at+cmgr=<index>		read sms message, respone: +cmgr:<>...<>\r\n<data>
	at+cmgd=<index>		delete sms message
	at+cmgd=?			response: +cmgd: <range of sms on sim>
	at+cnmi				new sms message indications

R:
	normal power down
	power low warning
	power low down
	rdy
	ok
	scks: 0				no simcard
*/
