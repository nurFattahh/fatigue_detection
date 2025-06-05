# Capstone Project
#### Kelas: **K2A** — Kelompok: **1**
#### Repository komputer edge gateway
[https://github.com/arvin-mf/capstone](https://github.com/arvin-mf/capstone)
# *Sistem Pemantauan Kelelahan Berbasis HRV Dan Suhu Tubuh dengan Implementasi pada Wearable Device*

## Deskripsi Proyek
Sistem ini melakukan deteksi kelelahan berdasarkan sinyal ECG dari tubuh serta temperatur tubuh. Kondisi kelelahan dideteksi dengan algoritma Support Vector Machine (SVM). Perangkat sistem memberikan peringatan dari buzzer setiap kali subyek terdeteksi mengalami kelelahan. Data-data numerik terkait kondisi subyek disimpan pada sebuah basis data dan ditampilkan dalam sebuah dashboard untuk monitoring.
<br><br>![Gambar perangkat wearable](https://i.postimg.cc/8csGtBjb/image-8.png)

## Alat yang Digunakan
1. Mikrokontroler, Sumber Daya, Sensor, dan Aktuator
- Mikrokontroler: ESP32
<br>![Gambar ESP32](assets/esp.png)<br>
- Sensor ECG: AD8232
<br>![Gambar AD8232](assets/sensor_ecg.png)<br>
- Sensor Suhu: MLX90614
<br>![Gambar MLX90614](assets/sensor_suhu.png)<br>
- Buzzer: 
<br>![Gambar ??](assets/buzzer.png)<br>
- Baterai: 
<br>![Gambar ??](assets/baterai.png)<br>

2. Dashboard Grafana
<br>![Gambar home page Grafana]()<br>

3. :whale:Docker 
Container-container yang akan diperlukan terdapat pada yaml docker-compose pada [repositori program komputer edge gateway](#repository-komputer-edge-gateway), antara lain:
- InfluxDB
<br>Digunakan sebagai penyimpanan data time series historikal dari pembacaan sensor-sensor pada setiap perangkat.
- :mosquito:Mosquitto
<br>Digunakan sebagai broker MQTT untuk menangani publish data dari ESP32 dan subscribe oleh komputer dashboard.
- MySQL
<br>Digunakan sebagai penyimpanan data relasional seputar alat/perangkat dengan subyek yang bekerja di tempat terkait.
- Migrate
<br>Digunakan untuk melakukan migrasi database antar komputer yang ingin menjadi komputer dashboard.
- Redis
<br>Digunakan untuk penyimpanan data sementara terkait status keaktifan perangkat.

## Arsitektur Sistem
Arsitektur sistem dapat diamati dari gambar berikut.
![Gambar diagram arsitektur](https://i.postimg.cc/VkPyCcBg/Arsitektur-Sistem-Kelompok1-drawio.png)

Sistem dapat terdiri dari satu komputer dashboard dan beberapa/banyak perangkat wearable pada subyek.<br>

Pada masing-masing perangkat pada subyek terdapat satu buah mikrokontroler ESP32, sensorAD8232, sensor MLX90614, serta buzzer. Sensor AD8232 dan buzzer melayani input/output kepada ESP32 melalui pin-pin GPIO. Sensor MLX90614 mengirimkan data input digital ke ESP32 melalui protokol I2C.<br>

Pada ESP32, terdapat bagian program yang melakukan inferensi status kelelahan dengan sejumlah parameter yang diproses dari masukan-masukan sensor. Bagian program tersebut merupakan implementasi dari bobot-bobot dan bias yang didapatkan dari algoritma SVM.<br>

ESP32 sebagai pusat pemrosesan di setiap perangkat pada subyek melakukan publish data ke broker MQTT pada server, atau pada proyek ini yaitu komputer yang menjadi edge gateway. Komputer edge gateway melakukan subscribe topik-topik dari semua ESP32 yang ada (menggunakan wildcard). Data yang diterima pada topik MQTT di-bind pada program di komputer edge gateway untuk diproses/disesuaikan dan disimpan ke database.<br>

Komputer edge gateway dipasangi Grafana untuk penampilan grafik besaran-besaran yang didapatkan dari setiap sensor pada perangkat yang digunakan oleh para subyek. Dashboard pada Grafana tersebut membaca data dari basis data InfluxDB yang menyimpan data time series.<br>

## Algoritma Machine Learning
### Support Vector Machine



## Dashboard

## 

