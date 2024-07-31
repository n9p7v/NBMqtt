#include <MKRNB.h>
#include "NBMqtt.h"

#define CLIENT_ID "clientId"
#define NVM_MODE 1
#define MQTT_PORT 1883
#define SERVER_IP "127.0.0.1"
#define KEEP_ALIVE 0
#define USERNAME "username"
#define PASSWORD "password"
#define QOS 0
#define RETAIN 0
#define TOPIC "test"
#define PAYLOAD "{\"key\":\"value\"}"

NBMqtt mqttClient(true); // true enables debugging

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
  

  // Set MQTT profile
  mqttClient.setMQTTProfileAuth(CLIENT_ID,MQTT_PORT,SERVER_IP,USERNAME,PASSWORD,0);

  // Connect to MQTT broker
  mqttClient.loginMQTTClient();
  bool status = false;
  while (!status) {
    // Publish a payload to a topic
    status = mqttClient.publishMQTTTopic(QOS,RETAIN,TOPIC,PAYLOAD);
  }
  // Disconnect from MQTT broker
  mqttClient.logoutMQTTClient();
}

void loop() {  
  if (Serial.available()) {
    SerialSARA.write(Serial.read());
  }

  if (SerialSARA.available()) {
    Serial.write(SerialSARA.read());
  }
}