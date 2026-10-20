// Converted from Arduino sketch: proj_LED_and_Photocell/proj_LED_and_Photocell.ino
#include <Arduino.h>

const int potentiometer_potPin = A1;
const int photocell_potPin = A2;
const int firstLedPin = 2;
const int ledCount = 6;

// Divider: 5V -> variable pot resistance -> A1 -> 10k resistor -> GND.
const float fixedResistance = 10000.0;
const float potMaxResistance = 10000.0;
const int ledOn = HIGH;
const int ledOff = LOW;

void setup() {
  Serial.begin(9600);
  for (int pin = firstLedPin; pin < firstLedPin + ledCount; pin++) {
    digitalWrite(pin, ledOff);
    pinMode(pin, OUTPUT);
  }
}

float calc_voltage(int reading) {
  return reading * (5.0 / 1023.0);
}

int count_lights(int reading) {
  // Vout / Vcc = Rfixed / (Rpot + Rfixed).
  // Protect against division by zero if the analog input reads zero.
  if (reading <= 0) {
    return ledCount;
  }
  float resistance = fixedResistance * (1023.0 / reading - 1.0);
  float position = constrain(resistance / potMaxResistance, 0.0, 1.0);

  // Seven regions: all off, then one through six LEDs on.
  return constrain((int)(position * (ledCount + 1)), 0, ledCount);
}

void check_brightness(int potentiometer_reading, float photocell_voltage) {
  int lightsOn = 0;
  if (photocell_voltage <= 1.5) {
    Serial.println("Dark");
    lightsOn = count_lights(potentiometer_reading);
  } else if (photocell_voltage < 4.0) {
    Serial.println("Daylight");
  } else {
    Serial.println("Bright");
  }

  for (int led = 0; led < ledCount; led++) {
    digitalWrite(firstLedPin + led, led < lightsOn ? ledOn : ledOff);
  }
  Serial.print("Pot voltage: ");
  Serial.print(calc_voltage(potentiometer_reading));
  Serial.print(" V, LEDs on: ");
  Serial.println(lightsOn);
}

void loop() {
  int potentiometer_reading = analogRead(potentiometer_potPin);
  int photocell_reading = analogRead(photocell_potPin);
  check_brightness(potentiometer_reading, calc_voltage(photocell_reading));
  delay(250);
}
