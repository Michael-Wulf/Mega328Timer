// Include timer library
#include <Mega328Timer.h>

// Define GPIO of led
#define PIN_LED 13

// Function to be called on timer interrupt
void blinkLED() {
  // Toggle led's state
  static byte state = 1;
  state ^= 1;
  digitalWrite(PIN_LED, state);
}

void setup() {
  // Set PIN_LED to output
  pinMode(PIN_LED, OUTPUT);
  
  // Initialize timer 2 to generate an interrupt every 0.016 sec
  // (16.000 us) and set function to be called on interrupt
  // Keep in mind that timer 2 is an 8-bit timer.
  // Running at 16 MHz and using the largest prescaler (1024) 
  // the maximum timer period is 16.384 ms
  attachTimerInterrupt(2, 16000, blinkLED);
}

void loop() {
  // Do nothing...
}