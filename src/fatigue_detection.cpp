#include "fatigue_detection.h"
#include "svm_model_fatigue.h" // Include generated model

// Normalisasi manual berdasarkan MinMax dari training (asumsi)
float normalize(float val, float min_val, float max_val) {
    return (val - min_val) / (max_val - min_val);
}

bool Inferensi(float bpm, float body_temp, float ambient_temp_val) {
    // Normalisasi input
    float bpm_norm = normalize(bpm, 40.0, 140.0);
    float body_temp_norm = normalize(body_temp, 35.5, 39.0);
    float ambient_temp_norm = normalize(ambient_temp_val, 20.0, 40.0);
    
    // Input data untuk inferensi
    float input_data[3] = {bpm_norm, body_temp_norm, ambient_temp_norm};
    
    // Melakukan inferensi dengan model
    float result = score(input_data);
    
    // Tentukan apakah deteksi kelelahan
    bool fatigue = (result >= 0.5) ? true : false;

    // Tampilkan hasil di serial monitor
    Serial.print("Fatigue Detected: ");
    Serial.println(fatigue ? "YES" : "NO");

    delay(2000); // Delay sebelum pengulangan berikutnya
    return fatigue;
}
