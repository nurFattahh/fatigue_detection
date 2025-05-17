#ifndef ECG_SENSOR_H
#define ECG_SENSOR_H

// Konstanta dan variabel global
#define MAX_SAMPLES 250

struct ECGSample {
  String timestamp;
  int raw_ecg;
};

extern const int ecgPin;
extern const float sampleRate;
extern const int integrationWindowSize;

extern int window[];
extern int windowIndex;
extern unsigned long lastPeakTime;

extern float bpm;

extern float y_low, y_high;
extern float prev_ecg, prev_diff;
extern float lp_alpha, hp_alpha;

extern long threshold;
extern bool peakDetected;

extern ECGSample ecgBuffer[MAX_SAMPLES];
extern int ecgBufferIndex;

// Fungsi
void setupECG();
void sampleECG();           // dipanggil tiap 4ms
int hitungRataRataBPM();  // dipanggil setelah 30 detik


#endif
