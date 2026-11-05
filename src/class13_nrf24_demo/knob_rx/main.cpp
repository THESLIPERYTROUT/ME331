#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

const int cePin = 9;
const int csnPin = 8;
const byte address[6] = "TRT42";
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
  // No delay here: the transmitter sets the pace this just checks for new data
  if (radio.available()) {
    int angV = 0;
    radio.read(&angV, sizeof(angV));
    myServo.write(angV);
  }
}
