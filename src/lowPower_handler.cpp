#include "lowpower_handler.h"

// Fungsi untuk masuk ke mode deep sleep selama X detik
void deepSleep(int seconds) {
  Serial.println("Masuk ke mode deep sleep...");
  esp_sleep_enable_timer_wakeup(seconds * 1000000);  // Menghitung waktu dalam mikrodetik
  esp_deep_sleep_start();  // Memasukkan ESP32 ke deep sleep
  Serial.println("ESP32 terbangun dari deep sleep.");
}
