// Settings shared by every nRF24 sketch, so both radios always match.
//
// nRF24L01 wiring (Arduino Mega):
//   VCC  -> 3.3V  (NOT 5V, 5V can damage the module)
//   GND  -> GND
//   CE   -> 9
//   CSN  -> 8
//   SCK  -> 52
//   MOSI -> 51
//   MISO -> 50
//   IRQ  -> not connected
#pragma once

#include <Arduino.h>

const int cePin = 9;
const int csnPin = 8;

// Pipe address: five characters, must be unique to our group.
const byte address[6] = "TRT42";
