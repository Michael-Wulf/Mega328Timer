/*******************************************************************************
 ***                                                                         ***
 *** filename:   Mega328Timer.h                                              ***
 ***                                                                         ***
 *** project:    General Arduino project                                     ***
 ***                                                                         ***
 *** created on: 2019-01-16                                                  ***
 ***                                                                         ***
 *** created by: Michael Wulf                                                ***
 ***             Cold Spring Harbor Laboratory                               ***
 ***             Kepecs Lab                                                  ***
 ***             wulf@cshl.edu                                               ***
 ***                                                                         ***
 *** version:    1.0.0                                                       ***
 ***                                                                         ***
 *** updated on: 2019-01-16                                                  ***
 ***                                                                         ***
 *** info:       Header-file for Mega328Timer.c                              ***
 ***             Simple usage of the timers 1 and 2 of the Atmel/Microchip   ***
 ***             Atmega 328 based Arduinos (Uno, Nano, Fio, Mini...)         ***
 ***             This library will not work on other Arduinos but can also   ***
 ***             be used in standard Atmel Studio C projects...              ***
 ***                                                                         ***
 *** Copyright (c) 2019 Michael Wulf, Cold Spring Harbor Laboratory.         ***
 *** All right reserved.                                                     ***
 ***                                                                         ***
 *** This library is free software; you can redistribute it and/or modify    ***
 *** it under the terms of the GNU Lesser General Public License as          ***
 *** published by the Free Software Foundation; either version 2.1 of the    ***
 *** License, or (at your option) any later version.                         ***
 ***                                                                         ***
 *** This library is distributed in the hope that it will be useful, but     ***
 *** WITHOUT ANY WARRANTY; without even the implied warranty of              ***
 *** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU       ***
 *** Lesser General Public License for more details.                         ***
 ***                                                                         ***
 *** You should have received a copy of the GNU Lesser General Public        ***
 *** License along with this library; if not, write to the Free Software     ***
 *** Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,                  ***
 *** MA  02110-1301  USA                                                     ***
 ******************************************************************************/
#ifndef MEGA328TIMER_H_
#define MEGA328TIMER_H_

#ifdef __cplusplus
extern "C" {
#endif

// check specified mcu - if it is not an ATmega328/P -> stop!
#if (   !defined(__AVR_ATmega328__)  \
     && !defined(__AVR_ATmega328P__))
  #error Unsupported mcu!
#endif


/*******************************************************************************
 *** Necessary includes
 ******************************************************************************/
#include <avr/interrupt.h>


/*******************************************************************************
 *** Function prototypes
 ******************************************************************************/
void attachTimerInterrupt(uint8_t timer_number, uint32_t microseconds, void (*attached_fct)(void));
void detachTimerInterrupt(uint8_t timer_number);

#ifdef __cplusplus
}
#endif

#endif //MEGA328TIMER_H_