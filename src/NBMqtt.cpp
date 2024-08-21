#include "NBMqtt.h"

NBMqtt::NBMqtt(bool debug) {
  if (debug) {
    MODEM.debug();
  }
}

NBMqtt::~NBMqtt() {}

/********** MQTT profile configuration +UMQTT **********/

bool NBMqtt::setMQTTClientID(const char *client_id) {
  if (client_id == nullptr) {
      return false;
  }

  char client_id_truncated[129];
  strncpy(client_id_truncated, client_id, sizeof(client_id_truncated) - 1);
  client_id_truncated[sizeof(client_id_truncated) - 1] = '\0';
  
  MODEM.sendf("AT+UMQTT=0,\"%s\"", client_id_truncated);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 0,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTPort(uint16_t local_port) {
  if (local_port > 65535) {
    return false;
  }
  
  MODEM.sendf("AT+UMQTT=1,%u", local_port);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 1,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTServerName(const char *server_name) {
  if (server_name == nullptr) {
    return false;
  }

  char server_name_truncated[129];
  strncpy(server_name_truncated, server_name, sizeof(server_name_truncated) - 1);
  server_name_truncated[sizeof(server_name_truncated) - 1] = '\0';

  MODEM.sendf("AT+UMQTT=2,\"%s\"", server_name_truncated);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 2,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTServerIP(const char *IP_address, uint16_t local_port) {
  if ((IP_address == nullptr) || (local_port > 65535)) {
      return false;
  }

  MODEM.sendf("AT+UMQTT=3,\"%s\",%u", IP_address, local_port);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 3,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTUsernamePassword(const char *username, const char *password) {
  if ((username == nullptr) || (password == nullptr)) {
    return false;
  }

  char username_truncated[31];
  char password_truncated[31];
  strncpy(username_truncated, username, sizeof(username_truncated) - 1);
  username_truncated[sizeof(username_truncated) - 1] = '\0';
  strncpy(password_truncated, password, sizeof(password_truncated) - 1);
  password_truncated[sizeof(password_truncated) - 1] = '\0';
  MODEM.sendf("AT+UMQTT=4,\"%s\",\"%s\"", username_truncated, password_truncated);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 4,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTKeepAlive(uint16_t keep_alive) {
  if (keep_alive > 65535) {
    return false;
  }

  MODEM.sendf("AT+UMQTT=10,%u", keep_alive);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 10,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTsecure(bool MQTT_secure, uint8_t USECMNG_profile) {
  MODEM.sendf("AT+UMQTT=11,%u,%u", MQTT_secure, USECMNG_profile);
  if (MODEM.waitForResponse(100, &response) == 1) {
    return response.startsWith("+UMQTT: 11,1");
  } else {
    return false;
  }
}

bool NBMqtt::setCommonMQTTProfile(const char *client_id,
                            uint16_t local_port,
                            const char *IP_address,
                            uint16_t keep_alive,
                            const char *username,
                            const char *password,
                            bool MQTT_secure,
                            uint8_t USECMNG_profile) {
  result = true;
  result &= setMQTTClientID(client_id);
  result &= setMQTTPort(local_port);
  result &= setMQTTServerIP(IP_address, local_port);
  if (username && password) {
    result &= setMQTTUsernamePassword(username, password);
  }
  result &= setMQTTKeepAlive(keep_alive);
  if (MQTT_secure) {
    result &= setMQTTsecure(MQTT_secure, USECMNG_profile);
  }
  return result;
}

bool NBMqtt::setMQTTProfile(const char *client_id,
                            uint16_t local_port,
                            const char *IP_address,
                            uint16_t keep_alive) {
  return setCommonMQTTProfile(client_id, local_port, IP_address, keep_alive, nullptr, nullptr, false, 0);
}

bool NBMqtt::setMQTTProfileAuth(const char *client_id,
                                uint16_t local_port,
                                const char *IP_address,
                                const char *username,
                                const char *password,
                                uint16_t keep_alive) {
  return setCommonMQTTProfile(client_id, local_port, IP_address, keep_alive, username, password, false, 0);
}

bool NBMqtt::setMQTTProfileSecure(const char *client_id,
                                  uint16_t local_port,
                                  const char *IP_address,
                                  uint16_t keep_alive,
                                  bool MQTT_secure,
                                  uint8_t USECMNG_profile) {
  return setCommonMQTTProfile(client_id, local_port, IP_address, keep_alive, nullptr, nullptr, MQTT_secure, USECMNG_profile);
}

/********** TODO: MQTT will topic configuration +UMQTTWTOPIC **********/

/********** TODO: MQTT will message configuration +UMQTTWMSG **********/

/********** Save/Restore MQTT profile from NVM +UMQTTNV **********/

bool NBMqtt::setMQTTNvmMode(uint8_t NVM_mode) {
  if (NVM_mode > 2) {
    return false;
  }

  MODEM.sendf("AT+UMQTTNV=%u", NVM_mode);
  if (MODEM.waitForResponse(100, &response) == 1) {
    // response1 = "+UMQTTNV: " + String(_NVM_mode) + ",1";
    return response.startsWith("+UMQTTNV: " + String(NVM_mode) + ",1");
  } else {
    return false;
  }
}

/********** MQTT command +UMQTTC **********/

bool NBMqtt::logoutMQTTClient(void) {
  MODEM.sendf("AT+UMQTTC=0");
  if (MODEM.waitForResponse(120000UL, &response) == 1) {
    return response.startsWith("+UMQTTC: 0,1");
  } else {
    return false;
  }
}

bool NBMqtt::loginMQTTClient(void) {
  MODEM.sendf("AT+UMQTTC=1");
  if (MODEM.waitForResponse(120000UL, &response) == 1) {
    return response.startsWith("+UMQTTC: 1,1");
  } else {
    return false;
  }
}

bool NBMqtt::publishMQTTTopic(uint8_t QoS, bool retain, const char *topic_name, char *message) {
  MODEM.sendf("AT+UMQTTC=2,%u,%d,\"%s\",\"%s\"", QoS, retain, topic_name, message);
  if (MODEM.waitForResponse(120000UL, &response) == 1) {
    return response.startsWith("+UMQTTC: 2,1");
  } else {
    return false;
  }
}

// TODO: MQTT publish a file to a topic

// TODO: MQTT subscribe to the specified topic filter

// TODO: MQTT unsubscribe from the specified topic filter

// TODO: MQTT read message

// TODO: MQTT verbose/terse Reception format

bool NBMqtt::pingMQTTServer(const char *IP_address) {
  if (IP_address == nullptr) {
    return false;
  }

  MODEM.sendf("AT+UMQTTC=8,\"%s\"", IP_address);
  if (MODEM.waitForResponse(120000UL, &response) == 1) {
    return response.startsWith("+UMQTTC: 8,1");
  } else {
    return false;
  }
}