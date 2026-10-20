// Converted from Arduino sketch: class1_blink/class1_blink.ino
#include <Arduino.h>

// Forward declarations (the Arduino IDE generated these automatically)
void dot();
void dash();

/*
  Blink

  Turns an LED on and off, singaling SOS in morse code

*/

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}


// the loop function runs over and over again forever
void loop() {
// S
  dot();
  dot();
  dot();

  delay(1000);
// O
  dash();
  dash();
  dash();

  delay(1000);
// S
  dot();
  dot();
  dot();

  delay(1000);

}


void dot() {
    int delay1;
    delay1 = 250;
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(delay1); 
    digitalWrite(LED_BUILTIN, LOW);   
    delay(delay1);   
}
void dash() {
    int delay1;
    delay1 = 500;
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(delay1); 
    digitalWrite(LED_BUILTIN, LOW);   
    delay(delay1);   
}
