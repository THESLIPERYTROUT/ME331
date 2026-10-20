// Converted from Arduino sketch: proj_DHT11/proj_DHT11.ino
#include <Arduino.h>

#include "DHT.h"
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Write exactly one row so shorter readings erase old characters.
void printRow(int row, const char *text) {
  lcd.setCursor(0, row);
  bool endOfText = false;
  for (int column = 0; column < 16; column++) {
    if (!endOfText && text[column] == '\0') {
      endOfText = true;
    }
    lcd.print(endOfText ? ' ' : text[column]);
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);
  printRow(0, "DHT11 starting");
  printRow(1, "Please wait...");
}

void loop() {
  delay(2000);
  float humidity = dht.readHumidity();
  float fahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(fahrenheit)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    printRow(0, "Sensor error");
    printRow(1, "Check DHT wiring");
    return;
  }

  float heatIndex = dht.computeHeatIndex(fahrenheit, humidity, true);

  char temperatureText[12];
  char humidityText[12];
  char heatIndexText[12];
  char rowText[40];
  dtostrf(fahrenheit, 5, 1, temperatureText);
  dtostrf(humidity, 3, 0, humidityText);
  dtostrf(heatIndex, 5, 1, heatIndexText);

  snprintf(rowText, sizeof(rowText), "T:%sF RH:%s%%", temperatureText, humidityText);
  printRow(0, rowText);
  snprintf(rowText, sizeof(rowText), "Feels: %sF", heatIndexText);
  printRow(1, rowText);

  Serial.print(F("Temperature: "));
  Serial.print(fahrenheit, 1);
  Serial.print(F(" F, Relative humidity: "));
  Serial.print(humidity, 0);
  Serial.print(F(" %, Heat index: "));
  Serial.print(heatIndex, 1);
  Serial.println(F(" F"));
}
