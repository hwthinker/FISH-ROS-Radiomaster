/* FishBot IMU - Get All Data (LSM6DS3 Only)
 * 
 * OPTIMIZED FOR LSM6DS3 CHIP
 * - No MPU6050 detection (chip confirmed as LSM6DS3)
 * - Cleaner, faster initialization
 * - Direct LSM6DS3 configuration
 * 
 * Accelerometer values are given as multiple of the gravity [1g = 9.81 m/s²]
 * Gyro values are given in deg/s
 * Angles are given in degrees
 *
 * License: MIT
 */

#include <Wire.h>
#include <MPU6050_light.h>

// FishBot I2C pins
#define SDA_PIN 18
#define SCL_PIN 19

MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup() {
  Serial.begin(115200);
  delay(2000);
  
  Serial.println("\n\n");
  Serial.println("╔═══════════════════════════════════════════════════╗");
  Serial.println("║   FishBot IMU - LSM6DS3 All Data Reader          ║");
  Serial.println("╚═══════════════════════════════════════════════════╝");
  Serial.println();
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  
  Serial.println("Initializing LSM6DS3 sensor at 0x6A...");
  
  byte status = mpu.begin();
  
  if (status != 0) {
    Serial.println();
    Serial.println("❌ Failed to initialize LSM6DS3!");
    Serial.println();
    Serial.println("Troubleshooting:");
    Serial.println("  1. Check power to sensor");
    Serial.println("  2. Verify I2C connections");
    Serial.println("  3. Run I2C Scanner to confirm 0x6A");
    Serial.println();
    
    while(1) delay(1000);
  }
  
  Serial.println("✅ LSM6DS3 initialized successfully!");
  Serial.println();
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("Calibrating offsets...");
  Serial.println("⚠️  Keep the sensor STILL on a flat surface!");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  
  delay(1000);
  
  // Calibrate gyro and accelerometer
  mpu.calcOffsets(true, true);
  
  Serial.println("✅ Calibration complete!");
  Serial.println();
  
  // Display offsets
  Serial.println("Calculated Offsets:");
  Serial.println("┌─────────────┬──────────┬──────────┬──────────┐");
  Serial.println("│   Sensor    │    X     │    Y     │    Z     │");
  Serial.println("├─────────────┼──────────┼──────────┼──────────┤");
  
  Serial.print("│ Gyroscope   │ ");
  printPadded(mpu.getGyroXoffset(), 8, 2);
  Serial.print(" │ ");
  printPadded(mpu.getGyroYoffset(), 8, 2);
  Serial.print(" │ ");
  printPadded(mpu.getGyroZoffset(), 8, 2);
  Serial.println(" │");
  
  Serial.print("│ Accelero    │ ");
  printPadded(mpu.getAccXoffset(), 8, 2);
  Serial.print(" │ ");
  printPadded(mpu.getAccYoffset(), 8, 2);
  Serial.print(" │ ");
  printPadded(mpu.getAccZoffset(), 8, 2);
  Serial.println(" │");
  
  Serial.println("└─────────────┴──────────┴──────────┴──────────┘");
  Serial.println();
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🎉 Ready! Starting continuous data reading...");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println();
  
  delay(1000);
  timer = millis();
}

void loop() {
  mpu.update();

  if (millis() - timer > 1000) {
    
    Serial.println("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    Serial.println("┃              LSM6DS3 SENSOR DATA                ┃");
    Serial.println("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    Serial.println();
    
    // Temperature
    Serial.print("🌡️  TEMPERATURE: ");
    Serial.print(mpu.getTemp(), 1);
    Serial.println(" °C");
    Serial.println();
    
    // Accelerometer
    Serial.println("📐 ACCELEROMETER (g):");
    Serial.print("   X: ");
    printSigned(mpu.getAccX(), 6, 3);
    Serial.print("   Y: ");
    printSigned(mpu.getAccY(), 6, 3);
    Serial.print("   Z: ");
    printSigned(mpu.getAccZ(), 6, 3);
    Serial.println();
    Serial.println();
    
    // Gyroscope
    Serial.println("🔄 GYROSCOPE (°/s):");
    Serial.print("   X: ");
    printSigned(mpu.getGyroX(), 7, 2);
    Serial.print("   Y: ");
    printSigned(mpu.getGyroY(), 7, 2);
    Serial.print("   Z: ");
    printSigned(mpu.getGyroZ(), 7, 2);
    Serial.println();
    Serial.println();
    
    // Accelerometer-only angles
    Serial.println("📊 ACC ANGLES (from accelerometer only) (°):");
    Serial.print("   X: ");
    printSigned(mpu.getAccAngleX(), 7, 2);
    Serial.print("   Y: ");
    printSigned(mpu.getAccAngleY(), 7, 2);
    Serial.println();
    Serial.println();
    
    // Filtered angles (complementary filter)
    Serial.println("🎯 FILTERED ANGLES (gyro + accel fusion) (°):");
    Serial.print("   X (Roll):  ");
    printSigned(mpu.getAngleX(), 7, 2);
    Serial.print("   Y (Pitch): ");
    printSigned(mpu.getAngleY(), 7, 2);
    Serial.print("   Z (Yaw):   ");
    printSigned(mpu.getAngleZ(), 7, 2);
    Serial.println();
    
    Serial.println();
    Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
    Serial.println();
    
    timer = millis();
  }
}

void printPadded(float value, int width, int decimals) {
  String str = String(value, decimals);
  while (str.length() < width) {
    str = " " + str;
  }
  Serial.print(str);
}

void printSigned(float value, int width, int decimals) {
  String str;
  if (value >= 0) {
    str = " " + String(value, decimals);
  } else {
    str = String(value, decimals);
  }
  while (str.length() < width) {
    str = " " + str;
  }
  Serial.print(str);
}
