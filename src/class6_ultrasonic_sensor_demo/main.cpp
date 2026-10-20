// Converted from Arduino sketch: class6_ultrasonic_sensor_demo/class6_ultrasonic_sensor_demo.ino
#include <Arduino.h>

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define ECHOPIN 9
#define TRIGPIN 8

void setup() {
  lcd.begin(16, 2);
  lcd.print("booting");

  Serial.begin(9600);

  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  delay(2500);
  lcd.clear();
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

void loop() {

  long duration = readUltrasonicDistance(TRIGPIN, ECHOPIN);

  float cm = 0.01723 * duration;
  float inches = cm / 2.54;

  Serial.print("Distance: ");
  Serial.print(cm);
  Serial.println(" cm");

  lcd.setCursor(0, 0);
  lcd.print(inches);
  lcd.print(" in       ");

  delay(1000);
}
