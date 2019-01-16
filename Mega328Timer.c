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
 *** info:       Simple usage of the timers 1 and 2 of the Atmel/Microchip   ***
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

/*******************************************************************************
 *** Necessary includes
 ******************************************************************************/
#include "Mega328Timer.h"


/*******************************************************************************
 *** Module specific defines
 ******************************************************************************/
#define TIMER1MAX 65536 // Timer 1 is a 16-bit timer 
#define TIMER2MAX 255   // Timer 2 is a  8-bit timer 


/*******************************************************************************
 *** Module specific types
 ******************************************************************************/
// Prototype for a callback function
typedef void (*timer_isr_callback) (void);


/*******************************************************************************
 *** Prototypes for module-internal functions
 ******************************************************************************/
void emptyTimerCallback(void);


/*******************************************************************************
 *** Module variables
 ******************************************************************************/
// Function pointer (callback) for TIMER1_COMPA_vect
static timer_isr_callback isr_ovf1 = emptyTimerCallback;
// Function pointer (callback) for TIMER2_COMPA_vect
static timer_isr_callback isr_ovf2 = emptyTimerCallback;
// Values for preloading the timer/counter registers
static uint16_t timer1Preload;
static uint8_t  timer2Preload;


/*******************************************************************************
 *** Functions
 ******************************************************************************/
 
// Calculate the value for the compare registers or to preload tcnt...
uint16_t calcTop(const uint32_t fosc,
                 const uint16_t prescaler,
				 const uint32_t time_top)
{
	uint16_t returnValue;
	// Calculation with "intelligent" rounding
	returnValue = ((((fosc/1000000) * time_top) + (prescaler/2))/(prescaler))-1;
	return (returnValue);
}

// Initialize specified timer and attach given function to the isr...
void attachTimerInterrupt(const uint8_t timer_number,
                          const uint32_t microseconds,
						  void (*attached_fct)(void))
{
	uint8_t  prescale;    // store the bitmask to set prescaler 
	uint32_t timer_ticks; // store the amount of timer ticks
	uint8_t  oldSREG;
	uint16_t preload;
	
	// If specified timer_number is invalid
	// -> immediately leave the function...
	if ( (timer_number < 1) || (timer_number > 2) )
		return;
	
	// Calculate how many timer ticks based on no prescaler are needed
	// to wait the specified amount of microseconds...
	timer_ticks = ((F_CPU/1000000) * microseconds)-1;
	
	if (timer_number == 1)
	{
		// Now check to what value the prescaler has to be set...
		if (timer_ticks < TIMER1MAX) {					// Prescaler =    1
			prescale  = (1 << CS10);
			timer_ticks  = calcTop(F_CPU, 1, microseconds);
		}
		else if ( (timer_ticks >>= 3) < TIMER1MAX ) {	// Prescaler =    8
			prescale  = (1 << CS11);
			timer_ticks  = calcTop(F_CPU, 8, microseconds);
		}
		else if ( (timer_ticks >>= 3) < TIMER1MAX ) {	// Prescaler =   64
			prescale  = (1 << CS11) | (1 << CS10);
			timer_ticks  = calcTop(F_CPU, 64, microseconds);
		}
		else if ( (timer_ticks >>= 2) < TIMER1MAX ) {	// Prescaler =  256
			prescale  = (1 << CS12);
			timer_ticks  = calcTop(F_CPU, 256, microseconds);
		}
		else if ( (timer_ticks >>= 2) < TIMER1MAX ) {	// Prescaler = 1024
			prescale  = (1 << CS12) | (1 << CS10);
			timer_ticks  = calcTop(F_CPU, 1024, microseconds);
		}
		else
		{
			timer_ticks = TIMER1MAX;
			prescale  = 0x05;	
		}
		preload = TIMER1MAX - timer_ticks;
	}
	else if (timer_number == 2)
	{
		// Now check to what value the prescaler has to be set...
		if (timer_ticks < TIMER2MAX) {					// Prescaler =    1
			prescale  = (1 << CS20);
			timer_ticks  = calcTop(F_CPU, 1, microseconds);
		}
		else if ( (timer_ticks >>= 3) < TIMER2MAX ) {	// Prescaler =    8
			prescale  = (1 << CS21);
			timer_ticks  = calcTop(F_CPU, 8, microseconds);
		}
		else if ( (timer_ticks >>= 2) < TIMER2MAX ) {	// Prescaler =   32
			prescale  = (1 << CS21) | (1 << CS20);
			timer_ticks  = calcTop(F_CPU, 8, microseconds);
		}
		else if ( (timer_ticks >>= 1) < TIMER2MAX ) {	// Prescaler =   64
			prescale  = (1 << CS22);
			timer_ticks  = calcTop(F_CPU, 64, microseconds);
		}
		else if ( (timer_ticks >>= 1) < TIMER2MAX ) {	// Prescaler =  128
			prescale  = (1 << CS22) | (1 << CS20);
			timer_ticks  = calcTop(F_CPU, 64, microseconds);
		}
		else if ( (timer_ticks >>= 1) < TIMER2MAX ) {	// Prescaler =  256
			prescale  = (1 << CS22) | (1 << CS21);
			timer_ticks  = calcTop(F_CPU, 256, microseconds);
		}
		else if ( (timer_ticks >>= 2) < TIMER2MAX ) {	// Prescaler = 1024
			prescale  = (1 << CS22) | (1 << CS21) | (1 << CS20);
			timer_ticks  = calcTop(F_CPU, 1024, microseconds);
		}
		else
		{
			timer_ticks = TIMER2MAX;
			prescale  = 0x07;	
		}
		preload = TIMER2MAX - timer_ticks;
	}
	
	// Save Status Register
	oldSREG = SREG;
	// Disable interrupts
	cli();
	
	// Set register corresponding to specified timer
	switch(timer_number)
	{
		case 1:
			// Reset TCCR1A
			TCCR1A = 0x00;
			// Reset TCCR1B
			TCCR1B = 0x00;
			// Set counter to preload value
			timer1Preload = preload & 0xFFFF;
			TCNT1  = timer1Preload;
			// Enable "Output Compare Interrupt A"
			TIMSK1 = (1 << TOIE1);
			// Attach callback function
			isr_ovf1 = attached_fct;
			// Set clock and start timer
			TCCR1B |= prescale;
			// Exit
			break;
		case 2:
			// Reset TCCRnA 
			TCCR2A = 0x00;
			// Reset TCCR1B
			TCCR2B = 0x00;
			// Set counter to preload value
			timer2Preload = preload & 0xFF;
			TCNT2  = timer2Preload;
			// Enable "Output Compare Interrupt A"
			TIMSK2 = (1 << TOIE2);
			// Attach callback function
			isr_ovf2 = attached_fct;
			// Set clock and start timer
			TCCR2B |= prescale;
			// Exit
			break;
	}
	// Restore Status Register
	SREG = oldSREG;
}

// Detach isr and deactivate specified timer
void detachTimerInterrupt(const uint8_t timer_number)
{
	uint8_t oldSREG;
	
	// Save Status Register
	oldSREG = SREG;
	// Disable interrupts
	cli();
	
	// Set isr callback (function pointer) of specified timer 
	// to an empty function and deactivate interrupt!
	switch(timer_number)
	{
		case 1:
			isr_ovf1 = emptyTimerCallback;
			TIMSK1 &= ~(1 << TOIE1);
			break;
		case 2:
			isr_ovf2 = emptyTimerCallback;
			TIMSK2 &= ~(1 << TOIE2);
			break;
	}
	// Restore Status Register
	SREG = oldSREG;
}

// Dummy callback function without any commands...
void emptyTimerCallback(void)
{
	// Do nothing
}


/*******************************************************************************
 *** ISR-Functions
 ******************************************************************************/
// ISR for Timer 1 Overflow Interrupt
ISR(TIMER1_OVF_vect)
{
	TCNT1  = timer1Preload;
	isr_ovf1();
}

// ISR for Timer 1 Overflow Interrupt
ISR(TIMER2_OVF_vect)
{
	TCNT2  = timer2Preload;
	isr_ovf2();
}