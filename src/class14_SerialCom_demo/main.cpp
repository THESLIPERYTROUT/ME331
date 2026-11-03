#include <Arduino.h>

int photoCellPin = A1;
int ledPin = 7;
int LedState = LOW;
unsigned long previousMillis = 0;
int R2 = 10000;


void setup() {
  Serial.begin(9600);
  pinMode(photoCellPin, INPUT); 
  pinMode(ledPin, OUTPUT);
}


float calc_voltage(int reading) {
  float ratio = 5.0 / 1023.0;
  return reading * ratio;
}

float calc_resistance(int reading, int R2) {
  float ratio = 5.0 / 1023.0;
  float voltage = reading * ratio;
  float resistance = R2 * ((5 - voltage) / voltage);
  return resistance;
}

void blink_led() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    LedState = (LedState == LOW) ? HIGH : LOW;
    digitalWrite(ledPin, LedState);
  }
}

int fadeLED(int ledPin) {
  static float fadeValue = 0; // Keeps its value between calls

  int currentValue = (int)fadeValue; // analogWrite only takes whole numbers
  analogWrite(ledPin, currentValue);

  fadeValue += 25.5;
  if (fadeValue > 255) {
    fadeValue = 0; // Past full brightness, start over
  }

  return currentValue;
}

void loop() {
  int reading = analogRead(photoCellPin);
  Serial.println(reading);


  float resistance = calc_resistance(reading, R2);
  Serial.println(resistance);
  Serial.println(millis());

  int finalFadeValue = fadeLED(ledPin);
  Serial.println(finalFadeValue);

  delay(500);
}
