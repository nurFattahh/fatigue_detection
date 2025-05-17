#include <Arduino.h>
#include <esp_sleep.h>
#include <esp_task_wdt.h>

#include "suhu_sensor.h"
#include "ecg_sensor.h"
#include "fatigue_detection.h"
#include "mqtt_handler.h"
#include "lowPower_handler.h"
#include "buzzer_notification.h"
#include "env.h"

unsigned long lastECGTime = 0;
unsigned long lastSuhuTime = 0;

int samplingTime = 5000; // ms
int sleepTime = 5; // detik

#define WDT_TIMEOUT 12

void setup() {
  Serial.begin(115200);
  analogReadResolution(10); 
  esp_task_wdt_init(WDT_TIMEOUT, true);  
  esp_task_wdt_add(NULL);  
  setupWiFi();
  setupMQTT();
  setupSuhu();
  setupECG();
  setupBuzzer();
  
  buzzerStart();
}

void loop() {
  if (millis() - lastECGTime >= 4) {
  lastECGTime = millis();
  sampleECG();
}

// Setelah 30 detik sampling selesai
if (millis() - lastSuhuTime >= samplingTime) {
  lastSuhuTime = millis();

  int averageBpm = hitungRataRataBPM(); // hitung dari RR intervals
  if (averageBpm > 0) {
    bacaSuhu(suhuObjek, suhuLingkungan);

    Serial.print("BPM: \t");
    Serial.print(averageBpm);
    Serial.print("Suhu Lingkungan: \t");
    Serial.print(suhuLingkungan);
    Serial.print(" °C\t");
    Serial.print("Suhu Objek: ");
    Serial.print(suhuObjek);
    Serial.println(" °C");

    if (PRODUCTION) {
      bool status = Inferensi(averageBpm, suhuObjek, suhuLingkungan);
      buzzerInference(status);

      sendFeaturetoBroker(esp32ID, averageBpm, suhuObjek, suhuLingkungan, status);
      delay(2000);

      deepSleep(sleepTime);
    } else {
      delay(500);
    }
    
  }
}

  
}
