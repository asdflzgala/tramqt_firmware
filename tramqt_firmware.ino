// 1. KHAI BÁO THƯ VIỆN
#include <Wire.h>             // Cho I2C
#include <DHT.h>              // Cho DHT22
#include <Adafruit_Sensor.h>  // Thư viện chung của Adafruit
#include <Adafruit_BMP280.h>  // Cho BMP280
#include <BH1750.h>           // Cho BH1750

// 2. ĐỊNH NGHĨA CÁC CHÂN (PIN)
// <-- Điều chỉnh các chân này cho phù hợp với kết nối của bạn
#define DHT_PIN 4       // Chân DATA của DHT22
#define MUA_PIN 34      // Chân Analog (A0) của cảm biến mưa
#define SDA_PIN 21      // Chân SDA cho I2C
#define SCL_PIN 22      // Chân SCL cho I2C

// 3. ĐỊNH NGHĨA CÁC ĐỊA CHỈ I2C
#define BMP280_ADDRESS  0x76  // Hoặc 0x77
#define BH1750_ADDRESS 0x23  // Hoặc 0x5C

// 4. ĐỊNH NGHĨA LOẠI CẢM BIẾN VÀ NGƯỠNG
#define DHT_TYPE DHT22
#define RAIN_THRESHOLD 500
// 5. KHỞI TẠO CÁC ĐỐI TƯỢNG CẢM BIẾN
DHT dht(DHT_PIN, DHT_TYPE);
Adafruit_BMP280 bmp; // I2C
BH1750 lightMeter;   // I2C

void setup() {
  // Bắt đầu Serial Monitor
  Serial.begin(115200);
  Serial.println("Bat dau kiem tra 4 cam bien");

  // Bắt đầu I2C cho BMP280 và BH1750
  Wire.begin(SDA_PIN, SCL_PIN);

  // Khởi động DHT22
  dht.begin();

  // Khởi động BMP280
  if (!bmp.begin(BMP280_ADDRESS)) {
    Serial.println("Khong tin thay BMP280.");
  } else {
    Serial.println("Cam bien BMP280 san sang.");
  }

  // Khởi động BH1750
  if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE, BH1750_ADDRESS)) {
    Serial.println("Khong tim thay cam bien BH1750.");
  } else {
    Serial.println("Cam bien BH1750 san sang");
  }

  // Cài đặt chân cảm biến mưa
  pinMode(MUA_PIN, INPUT);
  Serial.println("Cam bien mua san sang");

  Serial.println("---------------------------------");
}

void loop() {
  // Gọi các hàm đọc từng cảm biến
  getTempHum();
  getBMPData();
  getLightLevel();
  getMUAData();

  Serial.println("---------------------------------");

  // Đợi 2 giây trước khi đọc lại
  delay(2000);
}

// === CÁC HÀM ĐỌC CẢM BIẾN ===

// 1. HÀM ĐỌC DHT22
void getTempHum() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT22: Loi doc du lieu");
    return;
  }
  Serial.print("DHT22 - temperature: ");
  Serial.print(t);
  Serial.print(" *C, ");
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
}

// 2. HÀM ĐỌC BMP280
void getBMPData() {
  float temp = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25);

  if (isnan(temp) || isnan(pressure) || isnan(altitude)) {
    Serial.println("BMP280: Loi doc du lieu");
    return;
  }
  Serial.print("BMP280 - pressure: ");
  Serial.print(pressure / 100.0F); // Chuyển từ Pa sang hPa
  Serial.print(" hPa, ");
  Serial.print("Temperature (tu BMP): ");
  Serial.print(temp);
  Serial.println(" *C");
}

// 3. HÀM ĐỌC BH1750
void getLightLevel() {
  float lux = lightMeter.readLightLevel();

  if (isnan(lux)) {
    Serial.println("BH1750: Loi doc du lieu");
    return;
  }
  Serial.print("BH1750 - Cuong do anh sang: ");
  Serial.print(lux);
  Serial.println(" lux");
}

// 4. HÀM ĐỌC CẢM BIẾN MƯA
void getMUAData() {
  int rainValue = analogRead(MUA_PIN);

  Serial.print("MƯA   - gia tri ananlog: ");
  Serial.print(rainValue);

  if (rainValue < RAIN_THRESHOLD) {
    Serial.println("  ->  Mua");
  } else {
    Serial.println("  ->  No mua");
  }
}