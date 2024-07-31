#include "NBMqtt.h"

NBMqtt::NBMqtt(bool debug) {
  if (debug) {
    MODEM.debug();
  }
}

NBMqtt::~NBMqtt() {}

/********** MQTT profile configuration +UMQTT **********/

bool NBMqtt::setMQTTClientID(const char *client_id) {
  char client_id_truncated[129];
  strncpy(client_id_truncated, client_id, sizeof(client_id_truncated) - 1);
  client_id_truncated[sizeof(client_id_truncated) - 1] = '\0';
  MODEM.sendf("AT+UMQTT=0,\"%s\"", client_id_truncated);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 0,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTPort(uint16_t local_port) {
  _local_port = local_port;
  MODEM.sendf("AT+UMQTT=1,%u", _local_port);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 1,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTServerName(const char *server_name) {
  char server_name_truncated[129];
  strncpy(server_name_truncated, server_name, sizeof(server_name_truncated) - 1);
  server_name_truncated[sizeof(server_name_truncated) - 1] = '\0';
  MODEM.sendf("AT+UMQTT=2,\"%s\"", server_name_truncated);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 2,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTServerIP(const char *IP_address, uint16_t local_port) {
  _local_port = local_port;
  MODEM.sendf("AT+UMQTT=3,\"%s\",%u", IP_address, _local_port);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 3,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTUsernamePassword(const char *username, const char *password) {
  char username_truncated[31];
  char password_truncated[31];
  strncpy(username_truncated, username, sizeof(username_truncated) - 1);
  username_truncated[sizeof(username_truncated) - 1] = '\0';
  strncpy(password_truncated, password, sizeof(password_truncated) - 1);
  password_truncated[sizeof(password_truncated) - 1] = '\0';
  MODEM.sendf("AT+UMQTT=4,\"%s\",\"%s\"", username_truncated, password_truncated);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 4,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTKeepAlive(uint16_t keep_alive) {
  _keep_alive = keep_alive;
  MODEM.sendf("AT+UMQTT=10,%u", _keep_alive);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 10,1");
  } else {
    return false;
  }
}

bool NBMqtt::setMQTTsecure(bool MQTT_secure, uint8_t USECMNG_profile) {
  _MQTT_secure = MQTT_secure;
  _USECMNG_profile = USECMNG_profile;
  MODEM.sendf("AT+UMQTT=11,%u,%u", _MQTT_secure, _USECMNG_profile);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTT: 11,1");
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
  _local_port = local_port;
  _keep_alive = keep_alive;
  _MQTT_secure = MQTT_secure;
  _USECMNG_profile = USECMNG_profile;
  _result = true;
  _result &= setMQTTClientID(client_id);
  _result &= setMQTTPort(_local_port);
  _result &= setMQTTServerIP(IP_address, _local_port);
  if (username && password) {
    _result &= setMQTTUsernamePassword(username, password);
  }
  _result &= setMQTTKeepAlive(_keep_alive);
  if (_MQTT_secure) {
    _result &= setMQTTsecure(_MQTT_secure, _USECMNG_profile);
  }
  return _result;
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
  _NVM_mode = NVM_mode;
  MODEM.sendf("AT+UMQTTNV=%u", _NVM_mode);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    // _response1 = "+UMQTTNV: " + String(_NVM_mode) + ",1";
    return _response.startsWith("+UMQTTNV: " + String(_NVM_mode) + ",1");
  } else {
    return false;
  }
}

/********** MQTT command +UMQTTC **********/

bool NBMqtt::logoutMQTTClient(void) {
  MODEM.sendf("AT+UMQTTC=0");
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTTC: 0,1");
  } else {
    return false;
  }
}

bool NBMqtt::loginMQTTClient(void) {
  MODEM.sendf("AT+UMQTTC=1");
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTTC: 1,1");
  } else {
    return false;
  }
}

bool NBMqtt::publishMQTTTopic(uint8_t QoS, bool retain, const char *topic_name, char *message) {
  _QoS = QoS;
  _retain = retain;
  MODEM.sendf("AT+UMQTTC=2,%u,%d,\"%s\",\"%s\"", _QoS, _retain, topic_name, message);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTTC: 2,1");
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
  MODEM.sendf("AT+UMQTTC=8,\"%s\"", IP_address);
  if (MODEM.waitForResponse(100, &_response) == 1) {
    return _response.startsWith("+UMQTTC: 8,1");
  } else {
    return false;
  }
}