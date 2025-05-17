#ifndef FATIGUE_DETECTION_H
#define FATIGUE_DETECTION_H

#include <Arduino.h>

// Prototipe fungsi
float normalize(float val, float min_val, float max_val);
bool Inferensi(float bpm, float body_temp, float ambient_temp_val);

#endif
