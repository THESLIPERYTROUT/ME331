// Part 2 transmitter: reads a potentiometer and sends the servo angle over RF.
//
// Potentiometer wiring: outer pins -> 5V and GND, middle pin -> A0
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#include "../radio_config.h"

const int potPin = A0;
const int sendDelayMs = 15;  // Lower = smoother servo, same value as the Knob example

RF24 radio(cePin, csnPin);

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio not responding, check wiring");
    while (true) {}
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int potVal = analogRead(potPin);
  int angVal = map(potVal, 0, 1023, 0, 180);

  radio.write(&angVal, sizeof(angVal));
  delay(sendDelayMs);
}
