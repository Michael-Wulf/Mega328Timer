# Mega328Timer Library

Library that can be used in plain C programs or Arduino sketches for Atmel/Microchip ATmega328 AVR MCUs.

----
##Details
This library facilitates a convenient way of using overflow interrupts of the AVR's timer 1 (16-bit) and 2 (8-bit).
Only overflow interrupts will be used to keep this library compatible to the Servo and tone libraries.

The servo library uses the ATmega328's timer 1 (16-bit timer) and creates an Output Compare Match A interrupt (OC1A) whenever the timer reaches a specified value.
To keep this timer library even compatible with the servo library if it is not using timer 1, it cannot define the interrupt service routine (ISR) for the OC1A interrupt (it is already defined in the servo library)!
That's why this library uses the overflow interrupt and preloads the timer/counter register TCNT1 accordingly after raising the overflow interrupt.

The tone library uses the ATmega328's timer 2 (8-bit timer) and also creates an Output Compare Match A interrupt (OC2A) whenever the timer reaches a specified value.
To keep this timer library even compatible with the tone library if it is not using timer 2, it cannot define the interrupt service routine (ISR) for the OC2A interrupt (it is already defined in the tone library)!
That's why this library uses the overflow interrupt and preloads the timer/counter register TCNT2 accordingly after raising the overflow interrupt.

----
##Usage
Use the **attachTimerInterrupt** function to repeatedly call a specific function in a fixed interval.

    attachTimerInterrupt(timerNr, interval_us, function_ptr)
    
* timerNr: The number of the timer to be used. Valid values: 1, 2 
* interval_us: The interval in **microseconds** in which the interrupt has to be raised
* function_ptr: A pointer to the function (the name of the function) that has to be executed. This function must have no input arguments and no return arguments! 
Furthermore, keep in mind that this function will be called from inside the interrupt service routine (ISR). The amount of instructions should be very limited and not time-consuming commands should be placed here. 
Additionally, keep in mind that no calls to interrupt-related functions can be executed within inside an ISR (e.g. calls to interrupt-drive I2C libraries).

Example:

    void setup()
    {
      ...
      attachTimerInterrupt(1, 500000, timer1Fnc);
      ...
    }
    
    void timer1Fnc()
    {
      // Do something here but KEEP IT SHORT!
      // Note: Calls to interrupt-driven libraries (e.g. I2C-calls) will cause a deadlock and should not be used here!
    }
    
----
##Limitations
1. No interrupt-driven functions should be called from inside the timer function. This will cause to a deadlock because during the timer function, which is called from the timer's overflow interrupt service routine, all interrupts will be shortly disabled!

2. Because of #1, the amount of instructions and their complexity should be very limited!

3. Max interval length at a cpu clock of 16 MHz (typically for AVR-powerde Arduinos, except Arduino Fio and Mini @ 3.3 V):
 * For timer 1: 4.194304 s
 * For timer 2: 0.016384 s
 
4. Because of other interrupts that are usually enabled on an Arduino a timer interval of < 500 us (microseconds) is not recommended.

----
##Changelog
* 2019/01/16: Initial version