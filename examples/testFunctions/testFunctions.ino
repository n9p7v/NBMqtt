#include <MKRNB.h>
#include "NBMqtt.h"
#include "arduino_secrets.h"

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[] = SECRET_PINNUMBER;

#define DELAY_TIME 30000
#define CLIENT_ID "clientId"
#define NVM_SAVE 2
#define NVM_RESTORE 1
#define MQTT_PORT 1883
#define SERVER_IP "127.0.0.1"
#define KEEP_ALIVE 0
#define USERNAME "username"
#define PASSWORD "password"
#define QOS 0
#define RETAIN 0
#define TOPIC "test"

// Initialize library instance
NBClient client;
GPRS gprs;
NB nbAccess;
NBMqtt mqttClient();

void setup() {
  // NEVER EVER use RESET_N
  pinMode(SARA_RESETN, OUTPUT);
  digitalWrite(SARA_RESETN, LOW);

  // Send Poweron pulse
  pinMode(SARA_PWR_ON, OUTPUT);
  digitalWrite(SARA_PWR_ON, HIGH);
  delay(150);
  digitalWrite(SARA_PWR_ON, LOW);

  // Initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  SerialSARA.begin(115200);

  Serial.println("Starting Arduino web client.");
  // Connection state
  boolean connected = false;

  // After starting the modem with NB.begin() attach to the GPRS network with the APN, login and password
  while (!connected) {
    if ((nbAccess.begin(PINNUMBER) == NB_READY) &&
        (gprs.attachGPRS() == GPRS_READY)) {
      connected = true;

      // Set MQTT profile
      Serial.println("Configuring MQTT...");
      mqttClient.setMQTTClientID(CLIENT_ID);
      mqttClient.setMQTTPort(MQTT_PORT);
      mqttClient.setMQTTServerIP(SERVER_IP, MQTT_PORT);
      mqttClient.setMQTTUsernamePassword(USERNAME, PASSWORD);
      mqttClient.setMQTTKeepAlive(KEEP_ALIVE);

      // Save profile to memmory
      mqttClient.setMQTTNvmMode(NVM_SAVE);

    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
}

void loop() {
  // Restore proofile from memmory
  mqttClient.setMQTTNvmMode(NVM_RESTORE);

  // Connecting to MQTT broker
  Serial.println("Connecting...");
  mqttClient.loginMQTTClient();
  
  // Publish a payload to a topic
  Serial.println("Publish payload...");
  char payload[];
  status = mqttClient.publishMQTTTopic(QOS,RETAIN,TOPIC,payload);
  
  // Disconnecting from MQTT broker
  Serial.println("Disconnecting...");
  mqttClient.logoutMQTTClient();

  delay(DELAY_TIME);
}