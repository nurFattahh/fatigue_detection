#include <Arduino.h>
#include <esp_task_wdt.h>
#include "ecg_sensor.h"
#include "mqtt_handler.h"

// Konstanta
const int ecgPin = 34;
const float sampleRate = 250.0;
const int integrationWindowSize = 30;
const int maxBeats = 100;
float lp_alpha = 0.1;
float hp_alpha = 0.95;
long threshold = 350;

// Variabel filter & deteksi
int window[integrationWindowSize];
int windowIndex = 0;
float y_low = 0, y_high = 0;
float prev_ecg = 0, prev_diff = 0;
bool peakDetected = false;
unsigned long lastPeakTime = 0;

// RR interval buffer
unsigned long rrIntervals[maxBeats];
int rrIndex = 0;

// Inisialisasi
void setupECG() {
  for (int i = 0; i < integrationWindowSize; i++) window[i] = 0;
  rrIndex = 0;
  lastPeakTime = 0;
  peakDetected = false;
}

// Fungsi untuk sampling dan simpan RR interval
void sampleECG() {
  int raw_ecg = analogRead(ecgPin);
  sendSampletoBroker(esp32ID, raw_ecg);  // Kirim data mentah ke broker
  esp_task_wdt_reset();

  y_low = lp_alpha * y_low + (1 - lp_alpha) * raw_ecg;
  float highpassed = hp_alpha * (y_high + y_low - prev_ecg);
  prev_ecg = y_low;
  y_high = highpassed;

  float diff = highpassed - prev_diff;
  prev_diff = highpassed;

  float squared = diff * diff;

  window[windowIndex] = squared;
  windowIndex = (windowIndex + 1) % integrationWindowSize;

  long integrated = 0;
  for (int i = 0; i < integrationWindowSize; i++) {
    integrated += window[i];
  }
  integrated /= integrationWindowSize;

  if (integrated > threshold) {
    if (!peakDetected) {
      peakDetected = true;
      unsigned long now = millis();

      if (lastPeakTime != 0) {
        unsigned long interval = now - lastPeakTime;

        if (interval > 300 && interval < 2000 && rrIndex < maxBeats) {
          rrIntervals[rrIndex++] = interval;
        }
      }

      lastPeakTime = now;
    }
  } else {
    peakDetected = false;
  }
}

// Fungsi untuk menghitung rata-rata BPM setelah sampling
int hitungRataRataBPM() {
  if (rrIndex == 0) return -1;

  unsigned long total = 0;
  for (int i = 0; i < rrIndex; i++) {
    total += rrIntervals[i];
  }

  float avgRR = (float)total / rrIndex;
  float avgBPM = 60000.0 / avgRR;

  rrIndex = 0;  // Reset setelah dihitung
  return avgBPM;
}
