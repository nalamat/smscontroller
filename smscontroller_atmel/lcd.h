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

/*
 * ertebate micro ba alphanumeric lcd e 16x2,
 * dar halate 4 biti az 7 pine micro estefade mikonad,
 * baraye nameyeshe dade haye adadi va string
*/

#ifndef SMS_CONTROLLER_LCD_H
#define SMS_CONTROLLER_LCD_H

#define LCD_DPIN  PIND
#define LCD_DDDR  DDRD
#define LCD_DPORT PORTD
#define LCD_CPIN  PINC
#define LCD_CDDR  DDRC
#define LCD_CPORT PORTC

#define LCD_PIN  PINC
#define LCD_DDR  DDRC
#define LCD_PORT PORTC

#define LCD_RS   0
#define LCD_RW   1
#define LCD_EN   2

#define LCD_MODE_DATA_8 1
#define LCD_MODE_DATA_4 0

/*
 * only for reading, use lcd_goto to set pos
*/
extern char lcd_x;
extern char lcd_y;

void lcd_send_cmd(const char cmd, const bool wait2ms=false);
void lcd_send_char(const char ch);

void lcd_init(const char _lcd_mode_data=LCD_MODE_DATA_8);
void lcd_clear();
void lcd_goto(const char ln, const char col);
/*
 * dir > 0 : shift left
 * dir < 0 : shift right
*/
void lcd_shift(char dir);

void lcd_print(const char *str, const int waitms=0);
void lcd_print(int num, const int waitms=0);
void lcd_print(long num, const int waitms=0);

#endif
