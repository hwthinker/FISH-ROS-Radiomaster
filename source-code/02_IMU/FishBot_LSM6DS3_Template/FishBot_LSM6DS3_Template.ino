/*
  FishBot_LSM6DS3_Template.ino (tanpa ubah library)
  - Board  : ESP32 DOIT DevKit V1
  - I2C    : SDA=18, SCL=19
  - Sensor : LSM6DS3 (di-handle oleh library MPU6050_light yg auto-detect)
  - Reset yaw:
      * Tekan tombol BOOT (GPIO0) -> yaw di-nolkan (pakai offset)
      * Atau kirim huruf 'z' via Serial
*/

#include <Wire.h>
#include <MPU6050_light.h>

// ======================== Konfigurasi Hardware ========================
#define I2C_SDA_PIN        18
#define I2C_SCL_PIN        19
#define RESET_BTN_PIN       0   // Tombol BOOT pada DOIT DevKit V1 (pulldown = ditekan)

// ======================== Objek & Variabel Global =====================
MPU6050 mpu(Wire);

// Offset yaw agar tidak menyentuh field private angleZ
volatile float yawZero = 0.0f;

// Timing cetak (non-blocking)
unsigned long lastPrintMs = 0;
const unsigned long printIntervalMs = 100;  // 10 Hz

// Debounce tombol
unsigned long lastBtnChangeMs = 0;
bool lastBtnState = HIGH;
const unsigned long debounceMs = 30;

// ======================== Fungsi Utilitas =============================
void resetYaw() {
  // Simpan sudut saat ini sebagai nol baru
  yawZero = mpu.getAngleZ();
}

float getYaw() {
  // Yaw relatif terhadap yawZero
  return mpu.getAngleZ() - yawZero;
}

void maybeHandleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == 'z' || c == 'Z') {
      resetYaw();
      Serial.println(F("[INFO] Yaw di-reset (serial)."));
    }
  }
}

void maybeHandleButton() {
  bool state = digitalRead(RESET_BTN_PIN);      // HIGH = lepas (karena PULLUP), LOW = ditekan
  unsigned long now = millis();

  if (state != lastBtnState && (now - lastBtnChangeMs) > debounceMs) {
    lastBtnChangeMs = now;
    lastBtnState = state;

    if (state == LOW) { // ditekan
      resetYaw();
      Serial.println(F("[INFO] Yaw di-reset (tombol)."));
    }
  }
}

void printAllData() {
  // Bacaan sudut (deg)
  float roll  = mpu.getAngleX();
  float pitch = mpu.getAngleY();
  float yaw   = getYaw();

  // Suhu (jika tersedia di LSM6DS3 via library)
  float tempC = mpu.getTemp();

  Serial.print(F("ROLL: "));  Serial.print(roll, 2);
  Serial.print(F("  PITCH: ")); Serial.print(pitch, 2);
  Serial.print(F("  YAW: "));   Serial.print(yaw, 2);
  Serial.print(F("  TEMP: "));  Serial.print(tempC, 2);
  Serial.println(F(" C"));
}

// ======================== Setup & Loop ================================
void setup() {
  // Serial
  Serial.begin(115200);
  delay(50);
  Serial.println();
  Serial.println(F("FishBot_LSM6DS3_Template (offset yaw, no lib change)"));

  // I2C
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000); // 400kHz

  // Tombol reset yaw (BOOT) dengan pull-up internal
  pinMode(RESET_BTN_PIN, INPUT_PULLUP);

  // Init sensor
  byte status = mpu.begin();
  if (status != 0) {
    Serial.print(F("[ERR] MPU/LSM6DS3 begin() gagal. Kode: "));
    Serial.println(status);
    Serial.println(F("Periksa wiring & alamat I2C."));
    // tetap lanjut—terkadang begin() return non-zero tapi masih bisa update
    delay(500);
  } else {
    Serial.println(F("[OK] Sensor terdeteksi."));
  }

  // Stabilkan pembacaan awal sebentar
  Serial.print(F("[INFO] Stabilisasi awal... "));
  for (int i = 0; i < 200; i++) {  // ~1 s dengan delay(5)
    mpu.update();
    delay(5);
  }
  Serial.println(F("selesai."));

  // Set yaw awal sebagai nol referensi
  yawZero = mpu.getAngleZ();
  Serial.println(F("[INFO] Yaw dinolkan ke posisi saat ini."));
  Serial.println(F("Tip: kirim 'z' via Serial atau tekan tombol BOOT untuk reset yaw."));
}

void loop() {
  // Update data IMU sesering mungkin
  mpu.update();

  // Handle input
  maybeHandleSerial();
  maybeHandleButton();

  // Cetak periodik
  unsigned long now = millis();
  if (now - lastPrintMs >= printIntervalMs) {
    lastPrintMs = now;
    printAllData();
  }
}
