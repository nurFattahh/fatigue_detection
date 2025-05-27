#include "mqtt_handler.h"
#include "ecg_sensor.h"
#include "env.h"

#include <esp_task_wdt.h>

WiFiClient espClient;
PubSubClient client(espClient);
String esp32ID;

void setupWifi();
void setupMQTT();
void reconnectMQTT();
void sendFeaturetoBroker(String id, float bpm, float suhuObjek, float suhuLingkungan, bool status);
void sendSampletoBroker(String id, int rawECG);
void batchSendSampletoBroker(String id);

void setupWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
  esp_task_wdt_reset();
}

void setupMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);

  // Ambil MAC address
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  esp32ID = "";

  for (int i = 0; i < 6; i++) {
    if (mac[i] < 0x10) esp32ID += "0";
    esp32ID += String(mac[i], HEX);
    if (i < 5) esp32ID += ":";
  }

  Serial.print("ESP32 MAC Address (ID unik): ");
  Serial.println(esp32ID);
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void sendFeaturetoBroker(String id, float bpm, float suhuObjek, float suhuLingkungan, bool status) {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  String payload = "{";
  payload += "\"bpm\": " + String(bpm, 2) + ",";
  payload += "\"suhu_objek\": " + String(suhuObjek, 2) + ",";
  payload += "\"suhu_lingkungan\": " + String(suhuLingkungan, 2) + ",";
  payload += "\"status\": " + String(status);
  payload += "}";

  String topic = "esp32/" + id + "/discrete";
  client.publish(topic.c_str(), payload.c_str());

  Serial.println("Data terkirim via MQTT: ");
  Serial.println(payload);
}


void sendSampletoBroker(String id, int rawECG) {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  String payload = "{";
  payload += "\"raw_ecg\": " + String(rawECG);
  payload += "}";

  String topic = "esp32/" + id + "/continue";
  client.publish(topic.c_str(), payload.c_str());
}

void batchSendSampletoBroker(String id) {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();

  String payload = "{";
  for (int i = 0; i < MAX_SAMPLES; i++) {
    payload += "{";
    payload += "\"timestamp\":\"" + ecgBuffer[i].timestamp + "\",";
    payload += "\"raw_ecg\":" + String(ecgBuffer[i].raw_ecg);
    payload += "}";
    if (i != MAX_SAMPLES - 1) payload += ",";
  }
  payload += "}";

  String topic = "esp32/" + id + "/continue";
  client.publish(topic.c_str(), payload.c_str());

  Serial.println("Data batch terkirim via MQTT: ");
  Serial.println(payload);
}

String formatTimestamp(unsigned long ms) {
  unsigned long total_seconds = ms / 1000;
  unsigned int hours = (total_seconds / 3600) % 24;
  unsigned int minutes = (total_seconds / 60) % 60;
  unsigned int seconds = total_seconds % 60;
  unsigned int milliseconds = ms % 1000;

  char buffer[20];
  sprintf(buffer, "%02u:%02u:%02u.%03u", hours, minutes, seconds, milliseconds);
  return String(buffer);
}

void sendStatusToBroker(String id, String status) {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  
  String payload = "{";
  payload += "\"device_status\": \"" + status + "\"";
  payload += "}";

  String topic = "esp32/" + id + "/status";
  client.publish(topic.c_str(), payload.c_str(), true);  // retain=true agar status disimpan oleh broker
  Serial.println("Status MQTT terkirim: " + payload);
}


