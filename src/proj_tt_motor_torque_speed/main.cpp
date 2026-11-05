// ME 331 - TT motor torque-speed characterization
// Spins the TT motor at a fixed PWM so every payload is tested under the same
// drive conditions. Film the wheel, then log results in
// TT_motor_torque_speed.xlsx (same folder).
//
// Wiring: same as class 12 - ENA = 5, IN1 = 3, IN2 = 2
//
// Serial monitor commands (9600 baud):
//   f = wind up the weight
//   r = reverse, to unwind the string between runs
//   s = stop
// If the weight winds the wrong way, swap the two motor leads.

#include <Arduino.h>

const int ENA = 5;
const int IN1 = 3;
const int IN2 = 2;

const int TEST_PWM = 255;  // keep the same for every data point

// Auto-stop so a stall test can't cook the motor or driver if left running
const unsigned long MAX_RUN_MS = 10000;

bool running = false;
unsigned long runStartMs = 0;

void printHelp() {
  Serial.println("Commands: f = wind up, r = unwind, s = stop");
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  if (running) {
    Serial.print("Stopped after ");
    Serial.print((millis() - runStartMs) / 1000.0, 2);
    Serial.println(" s");
  }
  running = false;
}

void startMotor(bool windUp) {
  digitalWrite(IN1, windUp ? HIGH : LOW);
  digitalWrite(IN2, windUp ? LOW : HIGH);
  analogWrite(ENA, TEST_PWM);

  running = true;
  runStartMs = millis();
  Serial.println(windUp ? "Winding up" : "Unwinding");
}

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  stopMotor();

  Serial.println("TT motor torque-speed test");
  Serial.print("PWM: ");
  Serial.println(TEST_PWM);
  printHelp();
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case 'f': startMotor(true); break;
      case 'r': startMotor(false); break;
      case 's': stopMotor(); break;
      case '\r':
      case '\n':
        break;  // ignore line endings from the serial monitor
      default: printHelp();
    }
  }

  if (running && millis() - runStartMs >= MAX_RUN_MS) {
    Serial.println("Max run time reached");
    stopMotor();
  }
}
