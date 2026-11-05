// Cooling fan: DHT11 temperature sets the RGB LED color and DC motor speed.
//   Room temperature          -> blue,   motor off
//   Slightly above room temp  -> yellow, motor at 50%
//   Significantly above room  -> red,    motor at 100%
#include <Arduino.h>
#include "DHT.h"

const int DHTPIN = 7;
const int bluepin = 9;
const int redpin = 10;
const int greenpin = 8;
const int ENA = 5;
const int IN1 = 3;
const int IN2 = 2;
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Temperature limits in Fahrenheit. Adjust these to match the room.
const float ROOM_TEMP_MAX = 78.0;  // at or below this: room temperature
const float WARM_TEMP_MAX = 82.0;  // at or below this: slightly warm; above: hot

const int MOTOR_OFF = 0;
const int MOTOR_HALF = 155;  // 50% duty cycle
const int MOTOR_FULL = 255;  // 100% duty cycle

const unsigned long READ_INTERVAL_MS = 2000;  // DHT11 needs about 2 s between reads

// 0 = room temperature, 1 = slightly warm, 2 = hot
int Cooling_state = 0;

void setLed(bool red, bool green, bool blue) {
  digitalWrite(redpin, red);
  digitalWrite(greenpin, green);
  digitalWrite(bluepin, blue);
}

void applyCoolingState() {
  switch (Cooling_state) {
    case 0:
      setLed(false, false, true);  // blue
      analogWrite(ENA, MOTOR_OFF);
      break;
    case 1:
      setLed(true, true, false);  // red + green = yellow
      analogWrite(ENA, MOTOR_HALF);
      break;
    case 2:
      setLed(true, false, false);  // red
      analogWrite(ENA, MOTOR_FULL);
      break;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(bluepin, OUTPUT);
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Motor direction is fixed; only the speed on ENA changes.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, MOTOR_OFF);

  dht.begin();
}

void loop() {
  delay(READ_INTERVAL_MS);
  float fahrenheit = dht.readTemperature(true);

  // Keep the last LED color and motor speed if a reading fails.
  if (isnan(fahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  if (fahrenheit <= ROOM_TEMP_MAX) {
    Cooling_state = 0;
  } else if (fahrenheit <= WARM_TEMP_MAX) {
    Cooling_state = 1;
  } else {
    Cooling_state = 2;
  }
  applyCoolingState();

  Serial.print(F("Temperature: "));
  Serial.print(fahrenheit, 1);
  Serial.print(F(" F, State: "));
  Serial.println(Cooling_state);
}
