#include <Arduino.h>
#include <Servo.h>

const int ECHOPIN = 8, TRIGPIN = 9, servoPin = 6;
const int buttonPin = 2; 
const int scanStep = 10;
const unsigned long settleMs = 150, pingIntervalMs = 60, debounceMs = 30;
const unsigned long echoTimeoutUs = 12000;
const unsigned long trackingSettleMs = 25;
// Allow about 3 ms/degree plus one servo refresh before measuring a move.
const unsigned long servoMsPerDegree = 3;
unsigned long moveSettleMs = settleMs, lastPingMs = 0, holdMs = 0;
bool targetMissing = false;
enum Mode { IDLE, SCAN_AND_POINT, SCAN_AND_TRACK };
enum Phase { SETTLING, SAMPLING, HOLDING };
Mode mode = IDLE;
Phase phase = HOLDING;
Servo servo;
int angle = 90, direction = 1, nearestAngle = 90;
float nearestDistance = 0, samples[3];
int sampleCount = 0, validCount = 0;
unsigned long phaseStarted = 0, buttonChanged = 0;
int lastReading = HIGH, stableButton = HIGH;
const int trackingStep = 5;
bool targetLocked = false, trackingProbe = false;
int targetAngle = 90, probeIndex = 0, bestTrackingAngle = 90;
float targetDistance = 0, bestTrackingDistance = 0;

int probeAngle(int index) {
    if (index == 1) return max(0, targetAngle - trackingStep);
    if (index == 2) return min(180, targetAngle + trackingStep);
    return targetAngle;
}

void moveTo(int target) {
    unsigned long travelMs = abs(target - angle) * servoMsPerDegree + 20;
    moveSettleMs = max(trackingProbe ? trackingSettleMs : settleMs, travelMs);
    angle = target;
    servo.write(angle);
    phaseStarted = millis();
    phase = SETTLING;
    sampleCount = validCount = 0;
}

void startScan() {
    targetLocked = false;
    trackingProbe = false;
    nearestDistance = 0;
    direction = angle <= 90 ? 1 : -1;
    moveTo(direction == 1 ? 0 : 180);
}

void updateButton() {
    int reading = digitalRead(buttonPin);
    unsigned long now = millis();
    if (reading != lastReading) {
        buttonChanged = now;
        lastReading = reading;
    }
    if (now - buttonChanged < debounceMs || reading == stableButton) return;
    stableButton = reading;
    if (reading != LOW) return; // Advance once per press, never repeat on hold.
    mode = static_cast<Mode>((static_cast<int>(mode) + 1) % 3);
    if (mode == IDLE) {
        servo.write(90);
        angle = 90;
        phase = HOLDING;
        Serial.println(F("Mode 1: idle"));
    } else {
        Serial.println(mode == SCAN_AND_POINT
            ? F("Mode 2: scan once and point") : F("Mode 3: scan and track"));
        startScan();
    }
}

float readDistance() {
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN, LOW);
    unsigned long duration = pulseIn(ECHOPIN, HIGH, echoTimeoutUs);
    return duration == 0 ? 0 : duration * 0.01723f;
}

float filteredDistance() {
    if (validCount < 2) return 0; 
    for (int i = 1; i < validCount; ++i) {
        for (int j = i; j > 0 && samples[j] < samples[j - 1]; --j) {
            float temp = samples[j];
            samples[j] = samples[j - 1];
            samples[j - 1] = temp;
        }
    }
    return validCount == 3 ? samples[1] : (samples[0] + samples[1]) / 2;
}

void updateScan() {
    if (mode == IDLE) return;
    unsigned long elapsed = millis() - phaseStarted; 
    if (phase == HOLDING) {
        if (mode == SCAN_AND_TRACK && targetLocked && elapsed >= holdMs) {
            // Follow only near the acquired target; never restart the full sweep.
            trackingProbe = true;
            probeIndex = 0;
            bestTrackingDistance = 0;
            bestTrackingAngle = targetAngle;
            moveTo(probeAngle(probeIndex));
        }
        return;
    }
    if (phase == SETTLING) {
        if (elapsed < moveSettleMs) return;
        phase = SAMPLING;
    } else if (elapsed < pingIntervalMs) return;

    // Keep echo spacing even though tracking uses only one ping per angle.
    if (millis() - lastPingMs < pingIntervalMs) return;
    lastPingMs = millis();
    float distance = readDistance();
    if (distance > 0) samples[validCount++] = distance;
    phaseStarted = millis();
    if (++sampleCount < (trackingProbe ? 1 : 3)) return;
    if (!trackingProbe) distance = filteredDistance();
    if (trackingProbe) {
        // Reject echoes far from the previous range to reduce target switching.
        float tolerance = max(15.0f, targetDistance * 0.35f);
        if (distance > 0 && abs(distance - targetDistance) <= tolerance &&
            (bestTrackingDistance == 0 || distance < bestTrackingDistance)) {
            bestTrackingDistance = distance;
            bestTrackingAngle = angle;
        }
        if (++probeIndex < 3) {
            moveTo(probeAngle(probeIndex));
            return;
        }
        if (bestTrackingDistance > 0) {
            targetAngle = bestTrackingAngle;
            targetDistance = bestTrackingDistance;
            targetMissing = false;
        } else {
            if (!targetMissing) {
                Serial.println(F("Target lost; holding last direction and checking locally"));
            }
            targetMissing = true;
        }
        holdMs = abs(targetAngle - angle) * servoMsPerDegree + 20;
        angle = targetAngle;
        servo.write(angle);
        trackingProbe = false;
        phase = HOLDING;
        phaseStarted = millis();
        return;
    }
    if (distance > 0 && (nearestDistance == 0 || distance < nearestDistance)) {
        nearestDistance = distance;
        nearestAngle = angle;
    }
    int nextAngle = angle + direction * scanStep;
    if (nextAngle >= 0 && nextAngle <= 180) {
        moveTo(nextAngle);
        return;
    }
    if (nearestDistance > 0) {
        targetMissing = false;
        targetLocked = mode == SCAN_AND_TRACK;
        targetAngle = nearestAngle;
        targetDistance = nearestDistance;
        holdMs = abs(nearestAngle - angle) * servoMsPerDegree + 20;
        angle = nearestAngle;
        servo.write(angle);
        Serial.print(F("Nearest object at "));
        Serial.print(angle);
        Serial.print(F(" degrees, "));
        Serial.print(nearestDistance);
        Serial.println(F(" cm"));
    } else {
        angle = 90;
        servo.write(angle);
        Serial.println(F("No valid object detected; pointing forward"));
    }
    phase = HOLDING;
    phaseStarted = millis();
}

void setup() {
    Serial.begin(9600);
    pinMode(TRIGPIN, OUTPUT);
    pinMode(ECHOPIN, INPUT);
    pinMode(buttonPin, INPUT);
    servo.attach(servoPin);
    servo.write(90);
    Serial.println(F("Mode 1: idle"));
}

void loop() {
    updateButton();
    updateScan();
}
