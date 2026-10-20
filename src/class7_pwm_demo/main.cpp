// Converted from Arduino sketch: class7_PWM_demo/class7_PWM_demo.ino
#include <Arduino.h>


const int greenPin = 9, yellowPin = 10, redPin = 11;

void setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  Serial.begin(9600);
}

void fadeLED(int ledPin, const char* color) {
  Serial.print(color); Serial.println(" : fade in");
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
  Serial.println("Hold at 255 for one second");
  delay(1000);
  Serial.println("Fade out");
  for (int fadeValue = 255; fadeValue >= 0; fadeValue -= 5) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
}

void loop() {
  fadeLED(greenPin, "GREEN");
  fadeLED(yellowPin, "YELLOW");
  fadeLED(redPin, "RED");
}
