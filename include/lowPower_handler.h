#ifndef LOWPOWER_HANDLER_H
#define LOWPOWER_HANDLER_H

#include <Arduino.h>
#include <esp_sleep.h>

// Deklarasi fungsi untuk memasukkan ESP32 ke deep sleep
void deepSleep(int seconds);

#endif  // LOWPOWER_HANDLER_H
