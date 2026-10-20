#include <Arduino.h>

const int button1Pin = 11, button2Pin = 12, button3Pin = 13;
const int buzzerPin = 4;

void setup() {
  pinMode(button1Pin, INPUT); //Configure button pins as input
  pinMode(button2Pin, INPUT);
  pinMode(button3Pin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);
  int button3State = digitalRead(button3Pin);

  if (button1State == HIGH) {
    Serial.println("Button 1 pressed");
    tone(buzzerPin, 500); // play a tone at 500 Hz
  } else if (button2State == HIGH) {
    Serial.println("Button 2 pressed");
    tone(buzzerPin, 1000); // play a tone at 1000 Hz
  } else if (button3State == HIGH) {
    Serial.println("Button 3 pressed");
    tone(buzzerPin, 1500); // play a tone at 1500 Hz
  } else {
    noTone(buzzerPin); 
  }
  
  delay(100); // delay to debounce the buttons
}