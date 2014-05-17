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

#include <avr/io.h>
#include <util/delay.h>
#include "common.h"
#include "lcd.h"

char lcd_x = 0;
char lcd_y = 0;
char lcd_lines[] = { 0x80, 0xc0 };
char lcd_mode_data;

void lcd_send_cmd(const char cmd, const bool wait2ms)
{
	if (lcd_mode_data == LCD_MODE_DATA_4)
	{
		LCD_PORT &= ~(1<<LCD_RS | 1<<LCD_RW);

		LCD_DDR  |=  0xf0;
		LCD_PORT = ( LCD_PORT & 0x0f) | (cmd & 0xf0);

		LCD_PORT |=  (1<<LCD_EN);
		delay_us(20);
		LCD_PORT &= ~(1<<LCD_EN);

		delay_us(20);
		LCD_PORT = ( LCD_PORT & 0x0f) | ((cmd<<4));// & 0xf0);

		LCD_PORT |=  (1<<LCD_EN);
		delay_us(20);
		LCD_PORT &= ~(1<<LCD_EN);

		delay_us(100);
	}
	else if (lcd_mode_data == LCD_MODE_DATA_8)
	{
		LCD_CPORT &= ~(1<<LCD_RS | 1<<LCD_RW);

		LCD_DDDR  = 0xff;
		LCD_DPORT = cmd;

		LCD_CPORT |=  (1<<LCD_EN);
		delay_us(20);
		LCD_CPORT &= ~(1<<LCD_EN);

		delay_us(100);
	}

	if ( wait2ms ) delay_us(2000);
}

void lcd_send_data(const char data)
{
	if (lcd_mode_data == LCD_MODE_DATA_4)
	{
		LCD_PORT |=  (1<<LCD_RS);
		LCD_PORT &= ~(1<<LCD_RW);

		LCD_DDR  |=  0xf0;
		LCD_PORT = ( LCD_PORT & 0x0f) | (data & 0xf0);

		LCD_PORT |=  (1<<LCD_EN);
		delay_us(20);
		LCD_PORT &= ~(1<<LCD_EN);

		delay_us(20);
		LCD_PORT = ( LCD_PORT & 0x0f) | ((data<<4));// & 0xf0);

		LCD_PORT |=  (1<<LCD_EN);
		delay_us(20);
		LCD_PORT &= ~(1<<LCD_EN);

		delay_us(100);
	}
	else if (lcd_mode_data == LCD_MODE_DATA_8)
	{
		LCD_CPORT |=  (1<<LCD_RS);
		LCD_CPORT &= ~(1<<LCD_RW);

		LCD_DDDR  |= 0xff;
		LCD_DPORT = data;

		LCD_CPORT |=  (1<<LCD_EN);
		delay_us(20);
		LCD_CPORT &= ~(1<<LCD_EN);

		delay_us(100);
	}
}

void lcd_init(const char _lcd_mode_data)
{
	lcd_mode_data = _lcd_mode_data;

	delay_us(15000);

	// 4 bit, 2 lines, 5x10 dot font
	// increment, no shift on entry
	// display on, cursor on, blink off
	// clear
	if (lcd_mode_data == LCD_MODE_DATA_4)
	{
		LCD_DDR = 0xff;
		LCD_PORT = ~(1<<LCD_EN);

		lcd_send_cmd(0b00110010);
		lcd_send_cmd(0b00101110);
		lcd_send_cmd(0b00000110);
		lcd_send_cmd(1, true);
		lcd_send_cmd(0b00001110);
	}
	else if (lcd_mode_data == LCD_MODE_DATA_8)
	{
		LCD_CDDR  |= (1<<LCD_RS | 1<<LCD_RW | 1<<LCD_EN);
		LCD_CPORT &= ~(1<<LCD_EN);

		lcd_send_cmd(0b00111111);
		lcd_send_cmd(0b00001111);
		lcd_send_cmd(1, true);
		lcd_send_cmd(0b00000110);
	}

	lcd_x = 0;
	lcd_y = 0;
}

void lcd_clear()
{
	lcd_send_cmd(1, true);
	lcd_x = 0;
	lcd_y = 0;
}

void lcd_goto(const char x, const char y)
{
	lcd_x = x;
	lcd_y = y;

	lcd_send_cmd(lcd_lines[(int)y] + x);
}

void lcd_shift(char dir)
{
	if ( dir>0 )
		while ( dir-- )
			lcd_send_cmd(0b00011100);
	else
		while ( dir++ )
			lcd_send_cmd(0b00011000);
}

void lcd_print(const char *str, const int waitms)
{
	for ( int i=0 ; str[i] ; ++i )
	{
		lcd_send_data( str[i] );
		if ( waitms ) delay_ms(waitms);
	}
}

void lcd_print(int num, const int waitms)
{
	char buffer[20];
	char *bufferp = buffer;
	int len = 0;
	int temp = num;

	while ( temp )
	{
		++len;
		temp /= 10;
	}

	if ( !len )
	{
		buffer[0] = '0';
		buffer[1] = 0;
	}
	else while ( num )
	{
		buffer[len] = 0;
		for ( int i=len-1 ; i>=0 ; --i )
		{
			buffer[i] = '0' + num%10;
			num /= 10;
		}
	}

	while ( *bufferp )
	{
		lcd_send_data( *(bufferp++) );
		if ( waitms ) delay_ms(waitms);
	}
}

void lcd_print(long num, const int waitms)
{
	char buffer[20];
	char *bufferp = buffer;
	int len = 0;
	long temp = num;

	while ( temp )
	{
		++len;
		temp /= 10;
	}

	if ( !len )
	{
		buffer[0] = '0';
		buffer[1] = 0;
	}
	else while ( num )
	{
		buffer[len] = 0;
		for ( int i=len-1 ; i>=0 ; --i )
		{
			buffer[i] = '0' + num%10;
			num /= 10;
		}
	}

	while ( *bufferp )
	{
		lcd_send_data( *(bufferp++) );
		if ( waitms ) delay_ms(waitms);
	}
}
