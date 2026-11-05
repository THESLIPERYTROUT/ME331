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

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  const char text[] = "Hello World";
  bool delivered = radio.write(&text, sizeof(text));

  Serial.println(delivered ? "Sent" : "No acknowledgement from receiver");
  delay(1000);
}
