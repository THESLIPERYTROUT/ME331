#include <Arduino.h>

const int greenPin = 8, yellowPin = 9, redPin = 10;
const int buttonPin = 2;
const int buzzerPin = 3;

void setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("Button pressed");
    digitalWrite(greenPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, LOW);
    tone(buzzerPin, 1000); // play a tone at 1000 Hz
  } else {
    Serial.println("Button not pressed");
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(redPin, LOW);
    noTone(buzzerPin); 
  }
  delay(100); // delay to debounce the button
}