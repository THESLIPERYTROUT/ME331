// Part 2 receiver: moves the servo to each angle received over RF.
//
// Servo wiring: red -> 5V, brown/black -> GND, orange/yellow -> 6
// The Knob example uses pin 9 for the servo, but here pin 9 is the radio's CE.
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

#include "../radio_config.h"

const int servoPin = 6;

RF24 radio(cePin, csnPin);
Servo myServo;

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio not responding, check wiring");
    while (true) {}
  }

  myServo.attach(servoPin);

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  // No delay here: the transmitter sets the pace, and this just checks for new data.
  if (radio.available()) {
    int angV = 0;
    radio.read(&angV, sizeof(angV));
    myServo.write(angV);
  }
}
