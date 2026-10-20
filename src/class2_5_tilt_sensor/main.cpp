// Converted from Arduino sketch: class2.5_tilt_sensor/class2.5_tilt_sensor.ino
#include <Arduino.h>

const int tiltPin = 2;

void setup() {
  Serial.begin(9600);
  pinMode(tiltPin, INPUT_PULLUP);
}

void loop() {
  int switchState = digitalRead(tiltPin);

  if (switchState == LOW) {
    Serial.println("Closed");
  } else {
    Serial.println("Open");
  }

  delay(100);
}
