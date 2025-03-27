/*
 * This file is an extention of the MKR NB library.
 * Copyright (c) 2024 Nicholas Dimitriou. All rights reserved.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _NBMQTT_H
#define _NBMQTT_H

#include "Modem.h"

class NBMqtt {

public:
  /**
   * Constructor
   * 
   * @param debug - if true, dumps all AT dialogue to Serial
   * @return -
   */
  NBMqtt(bool debug = false);

  /**
   * Destructor
   */
  ~NBMqtt();

  /**
   * Set MQTT client ID with a limit of 128 characters
   * 
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTClientID(const char *client_id);

  /**
   * Set MQTT port number
   * 
   * @param 1883 non-TLS MQTT
   * @param 8883 TLS MQTT
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTPort(uint16_t local_port);

  /**
   * Set MQTT server name with a limit of 128 characters
   * 
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTServerName(const char *server_name);

  /**
   * Set MQTT server IP and port number
   * 
   * @param IP IPv4
   * @param 1883 non-TLS MQTT
   * @param 8883 TLS MQTT
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTServerIP(const char *IP_address, uint16_t local_port);

  /**
   * Set MQTT username and password with a limit of 30 characters each
   * 
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTUsernamePassword(const char *username, const char *password);
  
  /**
   * Set MQTT server keep alive time in seconds
   * 
   * @param 0 minimum (default)
   * @param 65535 maximum
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTKeepAlive(uint16_t keep_alive);

  /**
   * Set MQTT secure option
   * 
   * @param 0 no-TLS MQTT (default)
   * @param 1 TLS MQTT
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTsecure(bool MQTT_secure, uint8_t USECMNG_profile);

  /**
   * Save/Restore MQTT profile to/from non-volatile memory (NVM)
   * 
   * @param 0 restore default
   * @param 1 read profile
   * @param 2 save profile
   * @return - 1 = OK
   * @return - 0 = ERROR
   */
  bool setMQTTNvmMode(uint8_t NVM_mode);

  /**
   * Disconnect from MQTT server
   */
  bool logoutMQTTClient(void);

  /**
   * Connect to MQTT server
   */
  bool loginMQTTClient(void);

  /**
   * MQTT publish to a topic
   * 
   * @param QoS 0 (default) at most once delivery
   * @param QoS 1 at least once delivery
   * @param QoS 2 exactly once delivery
   * @param retain 0 (default) the message will not be retained by the MQTT broker
   * @param retain 1 the message will be retained by the MQTT broker
   * @param topic_name MQTT topic name to publish to
   * @param message Message for publishing
   */
  bool publishMQTTTopic(uint8_t QoS, bool retain, const char *topic_name, char *message);

  /**
   * Issue a ping to MQTT server
   * 
   * @param IP_address MQTT server IP
   */
  bool pingMQTTServer(const char *IP_address);
  
private:
  String response;
};

#endif // _NBMQTT_H