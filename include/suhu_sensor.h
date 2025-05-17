#ifndef SUHU_SENSOR_H
#define SUHU_SENSOR_H

#include <Wire.h>
#include <Adafruit_MLX90614.h>

// Deklarasi objek sensor dan variabel suhu
extern Adafruit_MLX90614 mlx;
extern float suhuObjek, suhuLingkungan;

// Deklarasi fungsi
void setupSuhu();
void bacaSuhu(float &suhuObjek, float &suhuLingkungan);

#endif  // SUHU_SENSOR_H
