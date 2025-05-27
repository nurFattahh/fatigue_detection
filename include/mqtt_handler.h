#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <WiFi.h>
#include <PubSubClient.h>

// Deklarasi objek client dan esp32ID
extern WiFiClient espClient;
extern PubSubClient client;
extern String esp32ID;

// Deklarasi fungsi untuk WiFi dan MQTT
void setupWiFi();
void setupMQTT();
void reconnectMQTT();
void sendFeaturetoBroker(String id, float bpm, float suhuObjek, float suhuLingkungan, bool status);
void sendSampletoBroker(String id, int rawECG);
String formatTimestamp(unsigned long ms);
void batchSendSampletoBroker(String id);
void sendStatusToBroker(String id, String status);

#endif  // MQTT_HANDLER_H
