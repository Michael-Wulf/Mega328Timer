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
  
  // Initialize timer 1 to generate an interrupt every 0.5 sec
  // (500.000 us) and set function to be called on interrupt
  attachTimerInterrupt(1, 500000, blinkLED);
}

void loop() {
  // Do nothing...
}
