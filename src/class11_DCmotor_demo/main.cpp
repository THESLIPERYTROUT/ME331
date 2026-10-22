#include <Arduino.h>

int potPin = A1;
int ENA = 4;
int IN1 = 3;
int IN2 = 2;    

/*
Truth Table for Motor Control:
IN1	IN2	Spinning Direction
Low(0)	Low(0)	Motor OFF
High(1)	Low(0)	Forward
Low(0)	High(1)	Backward
High(1)	High(1)	Motor OFF
*/

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  //turn off the motor initially
  digitalWrite(IN1, LOW);  
  digitalWrite(IN2, LOW);
}

float read_potentiometer(int reading, String return_type) {
    if (return_type == "voltage") {
        float ratio = 5.0 / 1023.0;
        return reading * ratio;
    } else if (return_type == "duty_cycle") {
        float ratio = 100.0 / 1023.0;
        return reading * ratio;
    } else {
        Serial.println("Invalid return type specified.");
        return -1; // Return an error value
    }
}

void loop() {
  int reading = analogRead(potPin);
  Serial.println(reading);

  float voltage = read_potentiometer(reading, "voltage");
  
  Serial.print("Voltage: ");
  Serial.print(reading);

 
  // Control motor direction based on potentiometer value
  if (reading < 450) { // Potentiometer in the lower half
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH); // Spin backward
    analogWrite(ENA, 128); // Set duty cycle to 50%
  } else if (reading > 573) { // Potentiometer in the upper half
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); // Spin forward
    analogWrite(ENA, 128); 
  } else { // Potentiometer at midpoint
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); // Turn off motor
    analogWrite(ENA, 0); // Set duty cycle to 0%
  }

  delay(100); 
}