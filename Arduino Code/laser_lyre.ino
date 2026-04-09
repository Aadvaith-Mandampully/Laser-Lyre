#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
const int numStrings = 8;
const int sensorPins[numStrings] = {A0, A1, A2, A3, A4, A5, A6, A7};
const int trackNumbers[numStrings] = {1, 2, 3, 4, 5, 6, 7, 8};
int threshold[numStrings];              // Auto-calibration values
bool beamBroken[numStrings] = {false};  // Track if beam is currently broken
void setup() {
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);
  Serial.println(F("Initializing DFPlayer Mini..."));
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("DFPlayer Mini not detected! Check wiring."));
    while (true);
  }
myDFPlayer.volume(25); // 0–30 volume range

  Serial.println(F("Calibrating sensors..."));
  for (int i = 0; i < numStrings; i++) {
    long sum = 0;
    for (int j = 0; j < 20; j++) {
      sum += analogRead(sensorPins[i]);
      delay(5);
    }
      int avg = sum / 20;
    threshold[i] = avg - 100; // Adjust the offset to your environment
    Serial.print(F("String "));
    Serial.print(i);
    Serial.print(F(" threshold: "));
    Serial.println(threshold[i]);
  }
}

void loop() {
  for (int i = 0; i < numStrings; i++) {
    int sensorValue = analogRead(sensorPins[i]);

    // Beam broken
    if (sensorValue < threshold[i] && !beamBroken[i]) {
      Serial.print(F("Beam broken: String "));
      Serial.println(i + 1);
      myDFPlayer.play(trackNumbers[i]);
      beamBroken[i] = true;
    }

    // Beam restored
    if (sensorValue >= threshold[i] && beamBroken[i]) {
      beamBroken[i] = false;
    }
  }
}



