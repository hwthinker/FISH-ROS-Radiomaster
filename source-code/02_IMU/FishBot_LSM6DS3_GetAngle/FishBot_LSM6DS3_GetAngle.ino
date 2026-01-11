/* FishBot IMU - Angle Reader (LSM6DS3 Only)
 * 
 * OPTIMIZED FOR LSM6DS3 CHIP
 * - Clean, fast initialization
 * - Compact angle display
 * - High update rate (100 Hz)
 * 
 * Shows Roll, Pitch, Yaw angles in degrees
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
  
  Serial.println("\n╔════════════════════════════════════════════╗");
  Serial.println("║  FishBot IMU - LSM6DS3 Angle Reader       ║");
  Serial.println("╚════════════════════════════════════════════╝\n");
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000);
  
  Serial.println("Initializing LSM6DS3...");
  
  byte status = mpu.begin();
  
  if (status != 0) {
    Serial.println("❌ Failed! Check connections.");
    while(1) delay(1000);
  }
  
  Serial.println("✅ LSM6DS3 connected!\n");
  
  Serial.println("Calibrating... Keep sensor STILL!");
  delay(1000);
  
  mpu.calcOffsets();
  
  Serial.println("✅ Calibration done!\n");
  
  Serial.println("Offsets:");
  Serial.printf("  Gyro:  X=%6.2f  Y=%6.2f  Z=%6.2f\n", 
                mpu.getGyroXoffset(), 
                mpu.getGyroYoffset(), 
                mpu.getGyroZoffset());
  Serial.printf("  Accel: X=%6.2f  Y=%6.2f  Z=%6.2f\n\n", 
                mpu.getAccXoffset(), 
                mpu.getAccYoffset(), 
                mpu.getAccZoffset());
  
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━");
  Serial.println("🎉 Ready! Reading angles at 100 Hz");
  Serial.println("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
  
  delay(500);
  timer = millis();
}

void loop() {
  mpu.update();
  
  // Print at 100 Hz (every 10ms)
  if ((millis() - timer) > 10) {
    
    Serial.print("Roll: ");
    Serial.print(mpu.getAngleX(), 1);
    Serial.print("°\t");
    
    Serial.print("Pitch: ");
    Serial.print(mpu.getAngleY(), 1);
    Serial.print("°\t");
    
    Serial.print("Yaw: ");
    Serial.print(mpu.getAngleZ(), 1);
    Serial.println("°");
    
    timer = millis();
  }
}
