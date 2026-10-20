// Converted from Arduino sketch: proj_tilt_sensor/proj_tilt_sensor.ino
#include <Arduino.h>

const int tiltPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(tiltPin, INPUT_PULLUP);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
}

void loop() {
  int switchState = digitalRead(tiltPin);

  if (switchState == LOW) {
    Serial.println("Closed");
    digitalWrite(6, LOW);
    digitalWrite(7, HIGH);
  } else {
    Serial.println("Open");
    digitalWrite(7, LOW);
    digitalWrite(6, HIGH);
  }

  delay(100);
}
