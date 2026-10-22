#include <Arduino.h>
#include <Servo.h>

int ECHOPIN = 8;
int TRIGPIN = 9;
int servoPin = 4;

Servo servo;

const int positions[] = {0, 45, 90, 135, 180};
const int length = sizeof(positions) / sizeof(positions[0]);
const unsigned long predictionIntervalMs = 2000;
const unsigned long echoTimeoutUs = 12000; 
const unsigned long servoSettleMs = 150; 
unsigned long nextPredictionMs;
bool scanForward = true;

const float objectDistanceCm = 20.0; // Adjuststable threshold for object detection

const char* classifyMeasurements(const float measurements[], int count) {
    bool left = false;
    bool front = false;
    bool right = false;

    for (int i = 0; i < count; i++) {
        if (measurements[i] <= 0 || measurements[i] > objectDistanceCm) {
            continue;
        }

        if (positions[i] < 90) {
            right = true;
        } else if (positions[i] > 90) {
            left = true;
        } else {
            front = true;
        }
    }

    if (left && front && right) return "dead end";
    if (left && right) return "in corridor";
    if (front && left) return "wall front and left";
    if (front && right) return "wall front and right";
    if (left) return "wall left";
    if (front) return "wall front";
    if (right) return "wall right";
    return "open";
}

void setup() {

  Serial.begin(9600);

    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
    servo.attach(servoPin);
    servo.write(positions[0]);
    delay(500); // Allow the initial move to finish before sampling.
    nextPredictionMs = millis() + predictionIntervalMs;
}


long readUltrasonicDistance(int triggerPin, int echoPin)
{
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  return pulseIn(echoPin, HIGH, echoTimeoutUs);
}

float readFilteredDistance() {
    float samples[3];
    int validCount = 0;
    for (int i = 0; i < 3; i++) {
        long duration = readUltrasonicDistance(TRIGPIN, ECHOPIN);
        if (duration > 0) {
            samples[validCount++] = 0.01723f * duration;
        }
        // Space pings to let echoes from the previous pulse fade.
        if (i < 2) delay(60);
    }

    // Require at least two echoes; missing echoes are not zero-distance objects.
    if (validCount < 2) return 0;
    for (int i = 1; i < validCount; i++) {
        for (int j = i; j > 0 && samples[j] < samples[j - 1]; j--) {
            float temp = samples[j];
            samples[j] = samples[j - 1];
            samples[j - 1] = temp;
        }
    }
    if (validCount == 2) return (samples[0] + samples[1]) / 2.0f;
    return samples[1]; // Median rejects one isolated high or low reading.
}

void loop() {
    float measurements[length];
    for(int step = 0; step < length; step++) {
        // Reverse each scan to avoid a long return move.
        int i = scanForward ? step : length - 1 - step;
        servo.write(positions[i]);
        delay(servoSettleMs);
        float cm = readFilteredDistance();
        Serial.print("Position: ");
        Serial.print(positions[i]);
        Serial.print(" degrees, ");
        Serial.print("Distance: ");
        Serial.print(cm);
        Serial.println(" cm");

        measurements[i] = cm;
    }
    scanForward = !scanForward;

    // Anchor predictions to a two-second schedule instead of adding a
    // two-second delay after the time already spent scanning.
    while ((long)(millis() - nextPredictionMs) < 0) {
        delay(1);
    }
    nextPredictionMs += predictionIntervalMs;

    Serial.print("Surroundings: ");
    Serial.println(classifyMeasurements(measurements, length));

}
