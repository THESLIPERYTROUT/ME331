// Converted from Arduino sketch: class2_led_challange/class2_led_challangev2/class2_led_challangev2.ino
#include <Arduino.h>

const unsigned long intervalb = 1000.0 / (2.0 * 6.0);
const unsigned long intervalr = 1000.0 / (2.0 * 4.0);
const unsigned long intervalg = 1000.0 / (2.0 * 2.0);

void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}

void blink_blue(unsigned long interval) {
  static unsigned long lastToggle = 0;
  static bool ledState = false;

  unsigned long now = millis();

  if (now - lastToggle >= interval) {
    lastToggle = now;
    ledState = !ledState;
    digitalWrite(7, ledState ? HIGH : LOW);
  }
}

void blink_red(unsigned long interval) {
  static unsigned long lastToggle = 0;
  static bool ledState = false;

  unsigned long now = millis();

  if (now - lastToggle >= interval) {
    lastToggle = now;
    ledState = !ledState;
    digitalWrite(6, ledState ? HIGH : LOW);
  }
}

void blink_green(unsigned long interval) {
  static unsigned long lastToggle = 0;
  static bool ledState = false;

  unsigned long now = millis();

  if (now - lastToggle >= interval) {
    lastToggle = now;
    ledState = !ledState;
    digitalWrite(5, ledState ? HIGH : LOW);
  }
}

void loop() {
  blink_blue(intervalb);
  blink_red(intervalr);
  blink_green(intervalg);
}
