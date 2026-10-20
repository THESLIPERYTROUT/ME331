// Converted from Arduino sketch: class5_lcd_demo/class5_lcd_demo.ino
#include <Arduino.h>

#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int potPin = A1;
int R2 = 1000;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  Serial.begin(9600);
  delay(1000);
  for (int pin = 2; pin <= 4; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
}

float calc_voltage(int reading) {
  float ratio = 5.0 / 1023.0;
  return reading * ratio;
}
float calc_resistance(int reading, int R2) {
  float ratio = 5.0 / 1023.0;
  float voltage = reading * ratio;
  float reistance = R2 * ((5 - voltage) / voltage);
  return reistance;
}


void loop() {
  
  int reading = analogRead(potPin);
  Serial.println(reading);
 
  float voltage = calc_voltage(reading);
  float resistance = calc_resistance(reading,R2);
  Serial.println(voltage);

  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  lcd.print(resistance);
  lcd.print(" ohms                                  ");
  lcd.setCursor(0, 1);
  delay(1000);
}
