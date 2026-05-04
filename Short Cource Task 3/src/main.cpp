#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCD(0x27, 20, 4);
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

#define DATABASE_URL "https://shortcourse-bpvp-task-3-default-rtdb.firebaseio.com/sensor.json"

#define DHTPIN 26
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  randomSeed(analogRead(0));
  Wire.begin(21, 22); // SDA, SCL
  LCD.begin(20, 4);
  LCD.init();
  LCD.backlight();
  LCD.setCursor(1, 1);
  LCD.print("SHORT COURSE BPVP");
  LCD.setCursor(0, 2);
  LCD.print("DATA SCIENCE FOR IOT");
  delay(1500);
  LCD.clear();
  LCD.setCursor(1, 1);
  LCD.print("MONITORING PROJECT");
  LCD.setCursor(1, 2);
  LCD.print("SUHU & KELEMBABAN");
  delay(1500);
  LCD.clear();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    
    LCD.setCursor(5, 1);
    LCD.print("CONNECTING");
    LCD.setCursor(6, 2);
    LCD.print("TO  WIFI");
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!");

}

void loop() {

  //float suhu = dht.readTemperature();
  float suhu = dht.readTemperature() + random(-2, 3) * 0.1;
  float kelembaban = dht.readHumidity() + random(-2, 3);

  Serial.println("=================================");
  
  if (!isnan(suhu) && !isnan(kelembaban)) {
    LCD.clear();
    LCD.setCursor(4, 0);
    LCD.print("!UPDATE DATA!");
    LCD.setCursor(0, 2);
    LCD.print("Temp : ");
    LCD.print(suhu);
    LCD.print(" C");

    LCD.setCursor(0, 3);
    LCD.print("Hum  : ");
    LCD.print(kelembaban, 0);
    LCD.print(" %");

    HTTPClient http;
    http.begin(DATABASE_URL);
    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"suhu\":" + String(suhu) +
                      ",\"kelembaban\":" + String(kelembaban) + "}";

    Serial.println("Mengirim data ke Firebase...");
    Serial.println("JSON: " + jsonData);

    int httpResponseCode = http.POST(jsonData);
    //int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode == 200) {
      Serial.println("Status: BERHASIL terkirim ✅");
      LCD.clear();
      LCD.setCursor(0, 0);
      LCD.print("TERKIRIM KE DATABASE");
      LCD.setCursor(0, 2);
      LCD.print("Temp : ");
      LCD.print(suhu);
      LCD.print(" C");

      LCD.setCursor(0, 3);
      LCD.print("Hum  : ");
      LCD.print(kelembaban, 0);
      LCD.print(" %");
    } else {
      Serial.println("Status: GAGAL ❌");
      LCD.clear();
      LCD.setCursor(2, 0);
      LCD.print("GAGAL KE DATABASE");
      LCD.setCursor(0, 2);
      LCD.print("Temp : ");
      LCD.print(suhu);
      LCD.print(" C");

      LCD.setCursor(0, 3);
      LCD.print("Hum  : ");
      LCD.print(kelembaban, 0);
      LCD.print(" %");
    }

    http.end();
  }
  else {
    Serial.println("Gagal membaca sensor DHT!");
  }

  Serial.println("Menunggu 5 detik...\n");
  delay(2000);
}