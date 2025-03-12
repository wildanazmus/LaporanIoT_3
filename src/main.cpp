#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

const uint8_t LEBAR_LAYAR = 128;
const uint8_t TINGGI_LAYAR = 64;
const int8_t PIN_RESET = -1;
const uint8_t ALAMAT_LAYAR = 0x3C;

const uint8_t PIN_SENSOR_DHT = 19;
const uint8_t PIN_SENSOR_CAHAYA = 32;
const uint8_t PIN_I2C_SDA = 21;
const uint8_t PIN_I2C_SCL = 22;

const unsigned long INTERVAL_PEMBARUAN = 2500;
unsigned long waktuSebelumnya = 0;

Adafruit_SSD1306 layarOLED(LEBAR_LAYAR, TINGGI_LAYAR, &Wire, PIN_RESET);
DHT sensorSuhu(PIN_SENSOR_DHT, DHT11);

void inisialisasiLayar();
void bacaDanTampilkanDataSensor();
void tampilkanNilaiSensor(float suhu, float kelembaban, int cahaya);

void setup() {
  Serial.begin(115200);
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  
  inisialisasiLayar();
  sensorSuhu.begin();
  
  Serial.println(F("Sistem berhasil diinisialisasi dan siap"));
}

void loop() {
  unsigned long waktuSekarang = millis();
  
  if (waktuSekarang - waktuSebelumnya >= INTERVAL_PEMBARUAN) {
    waktuSebelumnya = waktuSekarang;
    bacaDanTampilkanDataSensor();
  }
}

void inisialisasiLayar() {
  if (!layarOLED.begin(SSD1306_SWITCHCAPVCC, ALAMAT_LAYAR)) {
    Serial.println(F("Gagal menginisialisasi layar OLED"));
    while (true) {
      delay(100);  // Hentikan eksekusi tetapi izinkan tugas latar belakang ESP32
    }
  }
  
  layarOLED.clearDisplay();
  layarOLED.setTextColor(SSD1306_WHITE);
  layarOLED.display();
}

void bacaDanTampilkanDataSensor() {
  // Baca data sensor
  float suhu = sensorSuhu.readTemperature();
  float kelembaban = sensorSuhu.readHumidity();
  int tingkatCahaya = analogRead(PIN_SENSOR_CAHAYA);

  // Periksa apakah pembacaan valid
  if (isnan(suhu) || isnan(kelembaban)) {
    Serial.println(F("Gagal membaca dari sensor DHT!"));
    suhu = 0.0;
    kelembaban = 0.0;
  }
  
  tampilkanNilaiSensor(suhu, kelembaban, tingkatCahaya);
}

void tampilkanNilaiSensor(float suhu, float kelembaban, int cahaya) {
  layarOLED.clearDisplay();
  layarOLED.setTextSize(1);
  
  // Tampil suhu
  layarOLED.setCursor(5, 5);
  layarOLED.print(F("Suhu: "));
  layarOLED.print(suhu);
  layarOLED.println(F(" C"));
  
  // Tampil kelembaban
  layarOLED.setCursor(5, 25);
  layarOLED.print(F("Kelembaban: "));
  layarOLED.print(kelembaban);
  layarOLED.println(F("%"));
  
  // Tampi intensitas cahaya
  layarOLED.setCursor(5, 45);
  layarOLED.print(F("Cahaya: "));
  layarOLED.println(cahaya);
  
  layarOLED.display();
}