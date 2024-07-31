#include <Arduino.h>
#include "NBMqtt.h"

NBMqtt mqtt(true);

void setup() {
 // NEVER EVER use RESET_N
  pinMode(SARA_RESETN, OUTPUT);
  digitalWrite(SARA_RESETN, LOW);

  // Send Poweron pulse
  pinMode(SARA_PWR_ON, OUTPUT);
  digitalWrite(SARA_PWR_ON, HIGH);
  delay(150);
  digitalWrite(SARA_PWR_ON, LOW);

  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  SerialSARA.begin(115200);
}

void loop() {
 if (Serial.available()) {
    SerialSARA.write(Serial.read());
  }
  if (SerialSARA.available()) {
    Serial.write(SerialSARA.read());
  }
}
