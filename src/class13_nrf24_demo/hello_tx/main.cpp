// Part 1 transmitter: sends "Hello World" once per second.
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#include "../radio_config.h"

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
