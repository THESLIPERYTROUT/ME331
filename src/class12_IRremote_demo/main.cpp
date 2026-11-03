// ME 331 week 10a
// Control of DC motor with IR remote

//Help from...Lesson 13
//www.elegoo.com

#include <Arduino.h>
#include <IRremote.h>

const int ENA = 5;
const int IN1 = 3;
const int IN2 = 2;   
const int IR_PIN = 7;
const int REDLED = 22;
const int GREENLED = 24;
const int BLUELED = 26;

//for motor control
bool on = 0;
const int NUM_SPEEDS = 4;
const int SPEED_PWM[NUM_SPEEDS] = {100, 150, 200, 255};  //ENA duty for levels 1-4
int speedLevel = 0;  //index into SPEED_PWM, starts at level 1

//report the current level and update the motor right away if it is on
void applySpeed() {
  Serial.print("Speed level: ");
  Serial.println(speedLevel + 1);
  if (on) analogWrite(ENA, SPEED_PWM[speedLevel]);
}

void setup() {

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(BLUELED, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  IrReceiver.begin(IR_PIN);  // Start the receiver
}


void translateIR_tinker()  // takes action based on IR code received
{

  //map the IR code to the remote key
  switch (IrReceiver.decodedIRData.decodedRawData) {
    case 0xEA15FF00:
      Serial.println("DOWN");
      if (speedLevel > 0) speedLevel--;  //stop at level 1
      applySpeed();
      break;
    case 0xBC43FF00:
      Serial.println("RIGHT");
      if (on) {
        digitalWrite(IN1, HIGH);  //forward
        digitalWrite(IN2, LOW);
        analogWrite(ENA, SPEED_PWM[speedLevel]);
      }
      break;
    case 0xB946FF00:
      Serial.println("UP");
      if (speedLevel < NUM_SPEEDS - 1) speedLevel++;  //stop at level 4
      applySpeed();
      break;
    case 0xBF40FF00:
      Serial.println("OK");
      on = !on;   //toggle
      if (!on) {  //if not on, turn off motor & LED
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(REDLED, LOW);
      } else digitalWrite(REDLED, HIGH);  //motor is on--turn on LED
      break;
    case 0xBB44FF00:
      Serial.println("LEFT");
      if (on) {
        digitalWrite(IN1, LOW);  //reverse
        digitalWrite(IN2, HIGH);
        analogWrite(ENA, SPEED_PWM[speedLevel]);
      }
      break;
    case 0xAD52FF00: Serial.println("0"); break;
    case 0xE916FF00: Serial.println("1"); break;
    case 0xE619FF00: Serial.println("2"); break;
    case 0xF20DFF00: Serial.println("3"); break;
    case 0xF30CFF00: Serial.println("4"); break;
    case 0xE718FF00: Serial.println("5"); break;
    case 0xA15EFF00: Serial.println("6"); break;
    case 0xF708FF00: Serial.println("7"); break;
    case 0xE31CFF00: Serial.println("8"); break;
    case 0xA55AFF00: Serial.println("9"); break;
    case 0xBD42FF00: Serial.println("*"); break;
    case 0xB54AFF00: Serial.println("#"); break;
    default:
      Serial.println(" other button   ");
  }            // End Case
  delay(500);  // Do not get immediate repeat
}  

void loop() {
    if (IrReceiver.decode()) {

        Serial.print("Raw IR: 0x");
        Serial.println(
            IrReceiver.decodedIRData.decodedRawData,
            HEX
        );

        Serial.print("Command: 0x");
        Serial.println(
            IrReceiver.decodedIRData.command,
            HEX
        );

        translateIR_tinker();
        IrReceiver.resume();
    }
}