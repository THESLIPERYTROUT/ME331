// Converted from Arduino sketch: class8_servo_demo/class8_servo_demo.ino
#include <Arduino.h>

#include <Servo.h>

int servoPin = 5;
Servo servo;
int angle = 0; 
int positions[8] = {0,45,90,135,180,135,90,45};
int length = sizeof(positions) / sizeof(positions[0]);
int statusPin = 2;


void setup() {
  servo.attach(servoPin);
}

void loop() {
  for(int i = 0; i < length; i++) {
    servo.write(positions[i]);
    delay(350);
    if (positions[i] == 180) {
      digitalWrite(statusPin, HIGH);
      delay(2000);
      digitalWrite(statusPin, LOW);
    }
    
  }

}
