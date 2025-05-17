#include "suhu_sensor.h"

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float suhuObjek = 0, suhuLingkungan = 0;

void setupSuhu() {
  if (!mlx.begin()) {
    Serial.println("Gagal mendeteksi sensor MLX90614. Cek koneksi!");
    while (1);
  }
  Serial.println("Sensor MLX90614 terdeteksi!");
}

void bacaSuhu(float &suhuObjek, float &suhuLingkungan) {
  suhuObjek = mlx.readObjectTempC();
  suhuLingkungan = mlx.readAmbientTempC();
}
