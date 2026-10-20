// Converted from Arduino sketch: class4_light_detector_proj/class4_light_detector_proj.ino
#include <Arduino.h>

int potPin = A1;

void setup() {
  Serial.begin(9600);

  for (int pin = 2; pin <= 4; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

float calc_voltage(int reading) {
  float ratio = 5.0 / 1023.0;
  return reading * ratio;
}

void check_brightness(float voltage) {
  // Clear previously activated outputs.
  for (int pin = 2; pin <= 4; pin++) {
    digitalWrite(pin, LOW);
  }

  if (voltage <= 1.5) {
    Serial.println("Dark");
    digitalWrite(2, HIGH);
  } else if (voltage < 4.0) {
    Serial.println("Daylight");
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
  } else {
    Serial.println("Bright");
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
  }
}

void loop() {
  int reading = analogRead(potPin);
  Serial.println(reading);

  float voltage = calc_voltage(reading);
  Serial.println(voltage);

  check_brightness(voltage);

  delay(500);
}
