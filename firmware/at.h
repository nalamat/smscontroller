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

/*
 * tavabei baraye controle module'e sim300 ba
 * dasturati mansub be AT command,
 * az jomle ersal, daryaft, khandan va pak kardane sms,
 * in dasturat be vaseteye usart.h be surate serial
 * ferestade mishavand
*/

#ifndef __SMSCONTROLLER_AT_H__
#define __SMSCONTROLLER_AT_H__

void at_init();
void at_parse(const char *str);
void at_rsp_ready_set( void (*call)() );
void at_rsp_sms_sent_set( void (*call)(int index) );
void at_rsp_sms_sent_parse( const char *str );
void at_rsp_sms_read_set( void (*call)(const char *str, const char *num) );
void at_rsp_sms_read_parse( const char *str, bool header );
void at_cmd_sms_format();
void at_cmd_sms_send(const char *str, const char *num);
void at_cmd_sms_read(int index);
void at_cmd_sms_delete(int index);
void at_cmd_at();

#endif
