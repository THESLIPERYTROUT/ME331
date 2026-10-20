// Converted from Arduino sketch: class2_led_challange/class2_led_challange.ino
#include <Arduino.h>

void setup() { 
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
} 

void blink_blue(int delay_time) { 
  digitalWrite(2, HIGH); 
  delay(200); 
  digitalWrite(2, LOW); 
  delay(delay_time); 
} 

void blink_green(int delay_time) { 
  digitalWrite(3, HIGH); 
  delay(200); 
  digitalWrite(3, LOW); 
  delay(delay_time); 
} 

void blink_red(int delay_time) { 
  digitalWrite(4, HIGH); 
  delay(200); 
  digitalWrite(4, LOW); 
  delay(delay_time); 
} 

void loop() { 
  blink_blue(100); 
  blink_green(300); 
  blink_red(500); 
}
