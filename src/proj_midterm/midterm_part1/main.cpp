#include <Arduino.h>
#include <LiquidCrystal.h>

const int ECHOPIN = 8;
const int TRIGPIN = 9;
const int potPin = A1;
const int buttonPin = 2;
const int buzzerPin = 3;
const int redLedPin = 13;
const int greenLedPin = 12;
const int blueLedPin = 11;

void setup() {

  Serial.begin(9600);

    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
    pinMode(buttonPin, INPUT);
    pinMode(buzzerPin, OUTPUT);
    pinMode(redLedPin, OUTPUT);
    pinMode(greenLedPin, OUTPUT);
    pinMode(blueLedPin, OUTPUT);
}

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  return pulseIn(echoPin, HIGH);
}

void frequency_tone(int buzzerPin, int frequency, int duration) {
    int period = 1000000L / frequency; // Period in microseconds
    int pulseWidth = period / 2; // Pulse width for 50% duty cycle

    for (long i = 0; i < duration * 1000L; i += period) {
        digitalWrite(buzzerPin, HIGH);
        delayMicroseconds(pulseWidth);
        digitalWrite(buzzerPin, LOW);
        delayMicroseconds(pulseWidth);
    }
}


void loop() {

    long duration = readUltrasonicDistance(TRIGPIN, ECHOPIN);
    float cm = 0.01723 * duration;
    int beepPause = 400; // Silence between beeps, in milliseconds

    if (cm < 5) { //close case 
        digitalWrite(redLedPin, HIGH);
        digitalWrite(greenLedPin, LOW);
        digitalWrite(blueLedPin, LOW);
        frequency_tone(buzzerPin, 1000, 100);
        beepPause = 100; // Close: short pause makes beeps repeat faster
    } else if (cm < 20) { //midrange case 
        digitalWrite(redLedPin, LOW);
        digitalWrite(greenLedPin, HIGH);
        digitalWrite(blueLedPin, LOW);
        frequency_tone(buzzerPin, 1000, 100);
    } else { //far case
        digitalWrite(redLedPin, LOW);
        digitalWrite(greenLedPin, LOW);
        digitalWrite(blueLedPin, HIGH);
        noTone(buzzerPin); 
    }
    Serial.print("Distance: ");
    Serial.print(cm);
    Serial.println(" cm");

    delay(beepPause);
}
