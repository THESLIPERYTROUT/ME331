// Converted from Arduino sketch: class3_analog_controlled_blink/class_analog_controlled_blink/class_analog_controlled_blink.ino
#include <Arduino.h>

int potPin = 1;
float delay_time;


void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);

}

void blink_blue(int delay_time) { 
  digitalWrite(2, HIGH); 
  delay(200); 
  digitalWrite(2, LOW); 
  delay(delay_time); 
} 

int calc_delay(int reading) {
  float delay_time = reading / 2.0;
  return delay_time;
}


void loop() {
  int reading = analogRead(potPin);
  Serial.println(reading);
  float x = calc_delay(reading);
  blink_blue(x);
  
}
