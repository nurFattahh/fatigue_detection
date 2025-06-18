# Capstone Project
#### Kelas: **K2A** — Kelompok: **1**
Anggota Kelompok:
- Muhammad Aqil Fauzi &nbsp;&nbsp;&nbsp; (225150300111005)
- Muhammad Nur Fattah &nbsp; (225150300111015)
- Arvin Mulia Fernanda &nbsp;&nbsp;&nbsp;&nbsp; (225150300111018)
- Ariandamar Chairul P. &nbsp;&nbsp;&nbsp;&nbsp; (225150300111035)
- Fatyatulhaqq Diando N. &nbsp; (225150300111039)

#### Repository komputer edge gateway
[https://github.com/arvin-mf/capstone](https://github.com/arvin-mf/capstone)
# *Sistem Pemantauan Kelelahan Berbasis HRV Dan Suhu Tubuh dengan Implementasi pada Wearable Device*

## Deskripsi Proyek
Sistem ini melakukan deteksi kelelahan berdasarkan sinyal ECG dari tubuh serta temperatur tubuh. Kondisi kelelahan dideteksi dengan algoritma Support Vector Machine (SVM). Perangkat sistem memberikan peringatan dari buzzer setiap kali subyek terdeteksi mengalami kelelahan. Data-data numerik terkait kondisi subyek disimpan pada sebuah basis data dan ditampilkan dalam sebuah dashboard untuk monitoring.
<br><br>![Gambar perangkat wearable](https://i.postimg.cc/8csGtBjb/image-8.png)

## Alat yang Digunakan
1. Mikrokontroler, Sumber Daya, Sensor, dan Aktuator
- Mikrokontroler: ESP32
<br><img src="https://i.postimg.cc/CMrHcpH8/esp.png" alt="Gambar ESP32" width=200/><br>
- Sensor ECG: AD8232
<br><img src="https://i.postimg.cc/fTgKBJgT/ad8232.jpg" alt="Gambar AD8232" width=200/><br>
- Sensor Suhu: MLX90614
<br><img src="https://i.postimg.cc/bJ8HsbdL/mlx90614.jpg" alt="Gambar MLX90614" width=200/><br>
- Buzzer: 
<br><img src="https://i.postimg.cc/0QwjRH9B/baterai.jpg)" alt="Gambar baterai" width=200/><br>
<br><img src="https://i.postimg.cc/tJbq0qSs/charger.jpg)" alt="Gambar modul charger" width=200/><br>
- Baterai dan Modul Chargger: 
<br><img src="https://i.postimg.cc/x8pKx45F/buzzer.jpg)" alt="Gambar buzzer" width=200/><br>

2. :orange:Grafana
<br>Grafana dipasang pada komputer dashboard dan diakses melalui port `3000` (default) sebagai penampil visualisasi data-data yang tersimpan pada database.

3. :whale:Docker
<br>Container-container yang akan diperlukan terdapat pada yaml docker-compose pada [repositori program komputer edge gateway](#repository-komputer-edge-gateway), antara lain:
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

Berikut adalah wiring diagram dan casing perangkat keras pada setiap perangkat pada subyek.
![Gambar wiring diagram](https://i.postimg.cc/nzk5znVj/Diagram-Wiring-Capstone.png)
![Gambar desain casing](https://i.postimg.cc/hGJZnV6Y/3-D-Design-Casing.png)
<div>
    <span><img src="https://i.postimg.cc/7hNm99J5/Salinan-Whats-App-Image-2025-05-18-at-09-29-50-547e332f.jpg" alt="Gambar casing 1" width=200/></span>
    <span><img src="https://i.postimg.cc/WbCJKrmz/Salinan-Whats-App-Image-2025-05-18-at-09-29-53-e56d4ec0.jpg" alt="Gambar casing 2" width=200/></span>
</div><br>

Pada ESP32, terdapat bagian program yang melakukan inferensi status kelelahan dengan sejumlah parameter yang diproses dari masukan-masukan sensor. Bagian program tersebut merupakan implementasi dari bobot-bobot dan bias yang didapatkan dari algoritma SVM.<br>

ESP32 sebagai pusat pemrosesan di setiap perangkat pada subyek melakukan publish data ke broker MQTT pada server, atau pada proyek ini yaitu komputer yang menjadi edge gateway. Komputer edge gateway melakukan subscribe topik-topik dari semua ESP32 yang ada (menggunakan wildcard). Data yang diterima pada topik MQTT di-bind pada program di komputer edge gateway untuk diproses/disesuaikan dan disimpan ke database.<br>

Komputer edge gateway dipasangi Grafana untuk penampilan grafik besaran-besaran yang didapatkan dari setiap sensor pada perangkat yang digunakan oleh para subyek. Dashboard pada Grafana tersebut membaca data dari basis data InfluxDB yang menyimpan data time series.<br>

## Algoritma Machine Learning
### Support Vector Machine
Support Vector Machines (SVM) merupakan algoritma machine learning yang mengklasifikasikan data dengan mencari hyperplane sebagai garis optimal yang membedakan kelas yang ada pada data. Dalam pembentukannya, hyperplane paling optimal ditentukan dengan memaksimalkan margin yaitu jarak antara titik data terdekat dari kelas yang berlawanan.

#### Data yang digunakan
Berikut adalah gambar contoh pengambilan data pada coolterm.
![Gambar tangkapan layar file data](https://i.postimg.cc/pTmHg9JV/Salinan-Screenshot-2025-05-17-165408.png)<br>
Data untuk training dan pengujian terletak pada folder [/notebooks](https://github.com/nurFattahh/fatigue_detection/tree/master/notebooks)<br>

#### Visualisasi data berupa scatter
Gambar berikut menunjukkan visualisasi scatter dari data yang telah dinormalisasi. 
![Gambar scatter data](https://i.postimg.cc/d09nRnbg/Salinan-visualisasidata.png)


#### Training model dan pengujian
Training model dilakukan dengan menggunakan 80% dari jumlah baris data keseluruhan. 20% dari baris digunakan untuk pengujian.<br>
Hasil pengujian adalah sebagai berikut.
![confusion matrix](https://i.postimg.cc/jSzpCKqg/output.png)

#### Classification Report
- **Accuracy**: 0.98  
- **Precision**:
  - Kelas 0: 0.97  
  - Kelas 1: 1.00  
- **Recall**:
  - Kelas 0: 1.00  
  - Kelas 1: 0.89  
- **F1-Score**:
  - Kelas 0: 0.98  
  - Kelas 1: 0.94  
- **Macro Avg F1-Score**: 0.96  
- **Weighted Avg F1-Score**: 0.98  

Model menunjukkan performa yang sangat baik secara keseluruhan, dengan akurasi tinggi dan F1-score yang stabil. Recall pada kelas minoritas (kelas 1) sedikit lebih rendah, namun precision-nya tetap sempurna.
![classification report](https://i.postimg.cc/CxTyFd0n/Screenshot-2025-06-18-180108.png)

## Edge Gateway
### Broker MQTT
Sistem ini menggunakan Mosquitto sebagai broker MQTT yang dijalankan pada komputer edge gateway. Broker dijalankan berupa container Docker dengan image `eclipse-mosquitto:2` dengan port `1883`.
#### Topik-topik
Nama-nama topik yang digunakan pada sistem ini antara lain,
- `esp32/{MAC_ADDRESS}/discrete`
> untuk transmisi data pengukuran suhu dan hasil perhitungan bpm
- `esp32/{MAC_ADDRESS}/continue`
> untuk transmisi data sinyal raw ECG
- `esp32/{MAC_ADDRESS}/status`
> untuk transmisi data status keaktifan perangkat

Contoh penerimaan data dari subskripsi topik:
![Gambar data pada MQTTX](https://i.postimg.cc/rsQKYddT/Screenshot-2025-06-16-173918.png) 

### Interface Pengelola Data dari Sensor
Program interface melakukan tugas-tugas antara lain,
- melakukan subscribe terhadap topik-topik MQTT yang mendapatkan publish dari semua perangkat wearable di tempat
- menerima, mengubah, dan menyediakan data alokasi perangkat terhadap subyek/pekerja dari dashboard status
- melakukan penyesuaian data yang didapatkan dari subskripsi topik MQTT untuk kemudian menyimpannya pada database

Program interface ditulis dengan bahasa Go.<br>
Kode program dapat diakses pada [repositori ini](#repository-komputer-edge-gateway).

## Dashboard
### Dashboard Alokasi Perangkat dan Status Kelelahan
![Gambar dashboard status](https://i.postimg.cc/Y9JTQ82L/Screenshot-408-1.png)

### Dashboard Monitoring Kondisi Tubuh dan Lingkungan
![Gambar dashboard Grafana](https://i.postimg.cc/fW1MvjqK/Screenshot-405-1.png)

## Demo
Video demo produk dapat dilihat berikut ini.
[![Gambar](https://i.postimg.cc/mk1RfGcD/Screenshot-2025-06-17-212054.png)](https://drive.google.com/file/d/15xLARlAiJnPl_LpX3M2isx0Cvv_3rWRF/view?usp=drive_link)
