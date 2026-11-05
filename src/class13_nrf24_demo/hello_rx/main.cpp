#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

const int cePin = 9;
const int csnPin = 8;
const byte address[6] = "TRT42";

RF24 radio(cePin, csnPin);

void setup() {
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println("Radio not responding, check wiring");
    while (true) {}
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
  }
}
