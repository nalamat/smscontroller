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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "common.h"
#include "usart.h"
#include "lcd.h"
#include "at.h"
#include "adc.h"
#include "eep.h"
#include "queue.h"

char number[15] = "09369705583";
int number_eep = 0x00;
queue_int sms_list;
char clock_hour=16;
char clock_min=0;
volatile bool clock_updated = false;
bool power_critical = false;
volatile bool power_updated = false;

void timer_init()
{
	// Timer1: 0.125*64*62500 = 500,000us
	// OCR1A CTC mode, clk/64 prescaler
	TCNT1 = 0;
	OCR1A = 62500-1;
	TCCR1A = 0;
	TCCR1B = (1<<WGM12) | 0b011;

	TIMSK = 1<<OCIE1A;
}

void clock_update()
{
	int i=0;
	char buf[7];
	
	buf[i++] = ' ';
	buf[i++] = clock_hour/10 + '0';
	buf[i++] = clock_hour%10 + '0';
	buf[i++] = ':';
	buf[i++] = clock_min/10 + '0';
	buf[i++] = clock_min%10 + '0';
	buf[i++] = 0;

	lcd_goto(10, 1);
	lcd_print(buf);
}

void clock_advance()
{
	clock_min++;
	if ( clock_min/60 ) clock_hour++;
	clock_min %= 60;

	clock_hour %= 24;
}

void power_check()
{
	if ( clock_hour < 20 || 23 < clock_hour )
	{
		power_critical = false;
		return;
	}

	///////////////////////////////////////////
	bool critical = false;

	int test=0, count=0;
	for ( int i=0 ; i<5 ; ++i )
		if ( PORTB&(1<<i) )
			count++;

	adc_turn_on();
	test = adc_read(5);
	adc_turn_off();

	critical = count>3 || test>700;
	///////////////////////////////////////////

	if ( power_critical )
	{
		if ( !critical ) power_critical = false;
	}
	else if ( critical )
	{
		power_critical = true;

		cli();
		at_cmd_sms_send("Warning, power level critical ...", number);
		sei();

		lcd_goto(0, 1);
		lcd_print("S: Warning");
	}
}

void ready()
{
	delay_ms(1000);
	cli();
	at_cmd_sms_format();
	sei();
	//char send[9] = "AB 00000";
	//at_cmd_sms_send(send);
}

void sms_sent(int index)
{
	lcd_goto(0, 1);
	lcd_print("          ");
	lcd_goto(0, 1);
	lcd_print("Rec: ");
	lcd_print(index);

	if ( index<=0 || 50<=index ) return;

	sms_list.push(new int(index));
	at_cmd_sms_read(index);
}

void sms_read(const char *str, const char *num)
{
	int index = 0;
	if ( !sms_list.empty() )
	{
		index = *sms_list.top();
		sms_list.pop();
	}

	if ( !index ) return;

	lcd_goto(0, 0);
	lcd_print("                ");
	lcd_goto(0, 0);
	lcd_print("R");
	//lcd_print(index);
	lcd_print(":");
	lcd_print(str);

	int str_len = -1;
	while (str[++str_len]);

	// check if received message has the correct format
	if ( str_len>=8 &&
		(str[0]=='A' || str[0]=='a') &&
		(str[1]=='B' || str[1]=='b') &&
		str[2]==' ' )
	{
		if ( num[0] )
		{
			int i;
			for ( i=0 ; num[i] ; ++i ) number[i] = num[i];
			number[i] = 0;
			eep_write(number, sizeof number, number_eep);
		}

		char send[9] = "AB 00000";
		int test = 0;
		char out = 0;
		PORTB = 0;
		adc_turn_on();
		for ( int i=0 ; i<5 ; ++i )
		{
			out |= ((int)(str[3+i]=='1'))<<i;
			send[3+i] = str[3+i];
			PORTB = 1<<i;
			test = adc_read(i);
			if ( test<150 ) send[3+i] = 'x';

			//char buf[10];
			//itoa(test, buf, 10);
			//usart_send_str(buf);
			//usart_send_char('\r');
		}
		adc_turn_off();
		PORTB = out;

		//lcd_goto(1, 0);
		//lcd_print("                ");

		//lcd_goto(14, 0);
		//lcd_print("1");
		//delay_ms(5000);

		//lcd_goto(15, 0);
		//lcd_print("2");
		//delay_ms(5000);

		cli();
		at_cmd_sms_send(send, number);
		//lcd_goto(13, 1);
		//lcd_print("x");
		sei();

		lcd_goto(0, 1);
		lcd_print("S:");
		lcd_print(send);

		//lcd_goto(14, 1);
		//lcd_print("3");
		//delay_ms(5000);
	}

	cli();
	at_cmd_sms_delete(index);
	sei();

	//lcd_goto(15, 1);
	//lcd_print("4");
}

int main()
{
	DDRB  = 0b00011111;
	PORTB = 0b00011111;
	DDRD  = 0b10000000;
	PORTD = 0b00000000;
	delay_ms(300);

	//eep_write(number, 12, number_eep);
	eep_read(number, sizeof number, number_eep);

	timer_init();
	lcd_init(LCD_MODE_DATA_4);
	usart_init();
	adc_init();
	at_init();
	at_rsp_ready_set(ready);
	at_rsp_sms_sent_set(sms_sent);
	at_rsp_sms_read_set(sms_read);
	sei();

	PORTB = 0;
	
	//lcd_goto(0, 1);
	//lcd_print(number);

	while (1)
	{
		while ( usart_recv_buf.empty() && !clock_updated && !power_updated );

		if ( clock_updated )
		{
			clock_updated = false;
			clock_update();
		}
		if ( power_updated  )
		{
			power_updated = false;
			power_check();
		}
		if ( !usart_recv_buf.empty() )
		{
			char *str = usart_recv_buf.top();

			lcd_goto(0, 0);
			lcd_print("                ");
			lcd_goto(0, 0);
			lcd_print(str);

			at_parse(str);

			usart_recv_buf.pop();
		}
	}

	return 0;
}

int counter = 0;

ISR (TIMER1_COMPA_vect)
{
	PORTD ^= 0b10000000;
	counter++;
	if ( counter%2==0 )
	{
		//lcd_clear();
		//lcd_goto(0, 0);
		//lcd_print(counter/2);

		clock_advance();
		clock_updated = true;
	}
	if ( counter%20==0 )
	{
		power_updated = true;
	}
}
