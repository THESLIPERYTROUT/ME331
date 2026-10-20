// Converted from Arduino sketch: class7_PWM_demo/class7_PWM_demo_P2.ino/class7_PWM_demo_P2.ino.ino
#include <Arduino.h>

const int ledPin = 9;
const int potPin = A0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int potReading = analogRead(potPin);
  int fadeValue = map(potReading, 0, 1023, 0, 255);
  analogWrite(ledPin, fadeValue);

  Serial.print("potReading = ");
  Serial.print(potReading);
  Serial.print("  fadeValue = ");
  Serial.println(fadeValue);
  delay(100);
}
