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
 * vasete kar ba mobaddele analog be digitale micro,
 * shamele tavabei baraye rahandaziye avvalie, roshan
 * kardan, khamush kardan va khandane voltaje yek
 * pin az porte A
*/

#ifndef __SMS_CONTROLLER_ADC_H__
#define __SMS_CONTROLLER_ADC_H__

// initialize the adc peripheral without turning it on
void adc_init();

// power on the adc peripheral,
// make sure adc_init is called before adc_turn_on
void adc_turn_on();

// power of the adc peripheral
void adc_turn_off();

// read the digitized analog voltage of the desired pin on port a, pin can be 0
// through 7, the returned value is in range 0-1023 linearly mapped from 0v-5v
int adc_read(char pin);

#endif
