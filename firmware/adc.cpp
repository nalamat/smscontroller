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

#include <avr/io.h>
#include "adc.h"

void adc_init()
{
	// Vref=AVCC, right align
	ADMUX = 0b01<<REFS0 | 0b0<<ADLAR | 0b00000<<MUX0;
	// Fosc/128=62500Hz, 
	ADCSRA = 0b111<<ADPS0;
}

void adc_turn_on()
{
	ADCSRA |= 1<<ADEN;
}

void adc_turn_off()
{
	ADCSRA &= ~(1<<ADEN);
}

int adc_read(char pin)
{
	if ( !(ADCSRA&(1<<ADEN)) ) return 0;

	ADMUX = (ADMUX&0b11100000) | (pin<<MUX0);
	ADCSRA |= 1<<ADSC;

	while ( !(ADCSRA&(1<<ADIF)) );
	return ADC;
}
