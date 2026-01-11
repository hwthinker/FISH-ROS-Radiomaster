# 🔧 Troubleshooting Guide - FishBot LSM6DS3

Solusi lengkap untuk masalah yang sering terjadi.

---

## 📋 Index Masalah

- [Library Tidak Ditemukan](#1-library-tidak-ditemukan)
- [COM Port Tidak Muncul](#2-com-port-tidak-muncul)
- [Upload Gagal](#3-upload-gagal)
- [IMU Tidak Initialize](#4-imu-tidak-initialize)
- [Data Sensor Salah](#5-data-sensor-salah)
- [Serial Monitor Sampah](#6-serial-monitor-menampilkan-sampah)
- [Angles Tidak Berubah](#7-angles-tidak-berubah)
- [Yaw Drift Terus](#8-yaw-drift-terus)
- [Calibration Gagal](#9-calibration-gagal)
- [Data Noise/Tidak Stabil](#10-data-noisetidak-stabil)

---

## 1. Library Tidak Ditemukan

### Error Message:
```
MPU6050_light.h: No such file or directory
#include <MPU6050_light.h>
         ^~~~~~~~~~~~~~~~~
compilation terminated.
```

### Penyebab:
- Library tidak terinstall
- Library di folder salah
- Arduino IDE belum restart

### Solusi:

#### ✅ Step 1: Verifikasi Lokasi File

**Windows:**
```
C:\Users\[YourUsername]\Documents\Arduino\libraries\MPU6050_light\
├── MPU6050_light.h
└── MPU6050_light.cpp
```

**Mac:**
```
/Users/[YourUsername]/Documents/Arduino/libraries/MPU6050_light/
├── MPU6050_light.h
└── MPU6050_light.cpp
```

**Linux:**
```
/home/[YourUsername]/Arduino/libraries/MPU6050_light/
├── MPU6050_light.h
└── MPU6050_light.cpp
```

**Cek:**
- Folder harus bernama **exactly** `MPU6050_light` (case-sensitive di Mac/Linux)
- Harus ada **2 files** di dalam folder
- File harus di folder `libraries`, bukan `libraries/src`

#### ✅ Step 2: Restart Arduino IDE

**PENTING!** Library tidak akan terdeteksi tanpa restart!

1. Tutup **semua** window Arduino IDE
2. Tunggu 3 detik
3. Buka Arduino IDE lagi

#### ✅ Step 3: Verifikasi Instalasi

1. Open Arduino IDE
2. Go to: **Sketch → Include Library**
3. Scroll dan cari: **"MPU6050_light"**
4. ✅ Jika ada → Success!
5. ❌ Jika tidak ada → Ulangi Step 1 & 2

#### ✅ Step 4: Manual Include (Alternatif)

Jika masih gagal, copy **full path**:

```cpp
// Windows
#include "C:/Users/YourName/Documents/Arduino/libraries/MPU6050_light/MPU6050_light.h"

// Mac
#include "/Users/YourName/Documents/Arduino/libraries/MPU6050_light/MPU6050_light.h"

// Linux
#include "/home/YourName/Arduino/libraries/MPU6050_light/MPU6050_light.h"
```

---

## 2. COM Port Tidak Muncul

### Error Message:
```
Serial port not found
No upload port found
```

### Atau Tools → Port = (Kosong/Greyed out)

### Penyebab:
- Driver USB tidak terinstall
- Kabel USB rusak
- Board tidak terdeteksi

### Solusi:

#### ✅ Step 1: Install CP210x Driver (Windows)

1. Download dari: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
2. Pilih: **CP210x Windows Drivers**
3. Extract ZIP
4. Run: **CP210xVCPInstaller_x64.exe** (atau x86)
5. Follow installation wizard
6. **Restart computer**

#### ✅ Step 2: Check Device Manager (Windows)

1. Tekan: **Win + X** → Device Manager
2. Expand: **Ports (COM & LPT)**
3. Look for: **Silicon Labs CP210x USB to UART Bridge (COMx)**

**Jika ada:**
- ✅ Driver OK, note COM number (e.g., COM3)
- Pilih port ini di Arduino IDE

**Jika tidak ada atau ada warning (⚠️):**
- Right-click → Uninstall device
- Disconnect USB
- Reconnect USB
- Windows will reinstall driver

**Jika masih tidak muncul:**
- Kabel USB mungkin rusak (coba kabel lain)
- Board mungkin rusak

#### ✅ Step 3: Check USB Cable

**Test kabel:**
1. Try different USB cable (must be **data cable**, not charging-only)
2. Try different USB port (avoid USB hubs, use direct port)
3. If laptop: Try when plugged to power (some laptops limit USB power on battery)

**How to identify data cable:**
- Data cable: 4 wires (can transfer data + charge)
- Charging cable: 2 wires (charge only, NO data)
- Test: If cable can transfer files between phone and PC → it's data cable

#### ✅ Step 4: Linux Permissions

**If using Linux:**

```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER

# Apply changes
sudo usermod -a -G tty $USER

# Check if added
groups

# Log out and log back in (or reboot)
```

**Verify port exists:**
```bash
ls /dev/ttyUSB*
# or
ls /dev/ttyACM*
```

#### ✅ Step 5: Mac Troubleshooting

**Check if port exists:**
```bash
ls /dev/cu.*
```

**Should see something like:**
```
/dev/cu.usbserial-0001
/dev/cu.SLAB_USBtoUART
```

**If not found:**
- Install CP210x driver for Mac
- Restart Mac
- Check System Preferences → Security → Allow Silicon Labs driver

---

## 3. Upload Gagal

### Error Message:
```
A fatal error occurred: Failed to connect to ESP32
Timed out waiting for packet header
```

### Atau stuck di:
```
Connecting........._____....._____
```

### Penyebab:
- Board tidak masuk bootloader mode
- USB connection issue
- Wrong board settings

### Solusi:

#### ✅ Step 1: Manual Bootloader Mode

**Method yang PALING sering berhasil:**

1. **Prepare:** Click Upload di Arduino IDE
2. **Wait:** Sampai muncul "Connecting..."
3. **Hold BOOT:** Tahan tombol **BOOT** di board
4. **Keep holding:** Jangan lepas sampai muncul "Writing..."
5. **Release:** Lepas tombol BOOT
6. **Wait:** Biarkan upload selesai

**Visual guide:**
```
Arduino IDE: Click Upload
            ↓
Output:     Connecting........
            ↓
You:        [HOLD BOOT BUTTON]
            ↓
Output:     Writing at 0x00001000...
            ↓
You:        [RELEASE BOOT BUTTON]
            ↓
Output:     Done uploading
```

#### ✅ Step 2: Lower Upload Speed

1. Tools → Upload Speed → **115200** (dari 921600)
2. Try upload again
3. If works → stick with 115200

#### ✅ Step 3: Verify Board Settings

Check all settings di **Tools** menu:

| Setting | Correct Value |
|---------|---------------|
| Board | ESP32 Dev Module |
| Upload Speed | 115200 (or 921600) |
| CPU Frequency | 240MHz |
| Flash Frequency | 80MHz |
| Flash Mode | QIO |
| Flash Size | 4MB |
| Partition Scheme | Default 4MB with spiffs |
| Core Debug Level | None |
| Port | Your COM port |

#### ✅ Step 4: Try Different Port/Cable

- Try different USB port (prefer USB 2.0 over 3.0)
- Try different USB cable
- Avoid USB hubs

#### ✅ Step 5: Power Cycle

1. Disconnect USB
2. Wait 10 seconds
3. Reconnect USB
4. Try upload again

#### ✅ Step 6: Manual Reset After Upload

Some boards need manual reset:

1. Upload code (ignore timeout error if any)
2. Press **RESET** button on board
3. Code should start running

---

## 4. IMU Tidak Initialize

### Serial Monitor Shows:
```
Initializing LSM6DS3...
IMU Status: 1
❌ Failed to initialize LSM6DS3!
```

### Penyebab:
- Sensor tidak terdeteksi di I2C
- I2C pins salah
- Sensor rusak

### Solusi:

#### ✅ Step 1: Run I2C Scanner

Upload dan run: **I2C_Scanner_Diagnostic.ino**

**Expected output:**
```
Device 1: 0x3C → OLED Display
Device 2: 0x6A → LSM6DS3 IMU  ← HARUS ADA INI!
```

**If 0x6A NOT found:**
- Sensor mungkin rusak
- Or address different (check 0x6B)
- Or I2C bus issue

#### ✅ Step 2: Verify I2C Pins in Code

```cpp
// Check these lines:
#define SDA_PIN 18  // Must be 18!
#define SCL_PIN 19  // Must be 19!

// Later in setup():
Wire.begin(SDA_PIN, SCL_PIN);  // Must be called!
```

#### ✅ Step 3: Check Sensor Power

**If sensor has LED:**
- LED should be ON
- If OFF → no power to sensor

**Use multimeter:**
```
RED probe   → VCC pin of sensor
BLACK probe → GND pin of sensor
Expected: 3.3V (±0.2V)

If 0V:  → No power
If 5V:  → WRONG! Use 3.3V only!
If 2.5V: → Weak connection
```

#### ✅ Step 4: Try Different Address

Some LSM6DS3 variants use 0x6B:

```cpp
// In library or code, try:
#define LSM6DS3_ADDR 0x6B  // Instead of 0x6A
```

Or modify code to scan both:

```cpp
byte addresses[] = {0x6A, 0x6B};
for (byte addr : addresses) {
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0) {
    Serial.print("Found at: 0x");
    Serial.println(addr, HEX);
  }
}
```

#### ✅ Step 5: Power Cycle Board

1. Disconnect USB
2. Wait 10 seconds (capacitors discharge)
3. Reconnect USB
4. Try again

---

## 5. Data Sensor Salah

### Serial Monitor Shows:
```
Acc: 0.000g  0.000g  0.000g
Gyro: 0.0°/s  0.0°/s  0.0°/s
Angles: 0.0°  0.0°/s  0.0°/s
```

### Atau semua nilai = -1, 255, NaN

### Penyebab:
- Sensor tidak merespon
- Calibration gagal
- Wrong register configuration

### Solusi:

#### ✅ Step 1: Re-calibrate

**IMPORTANT: Calibration harus di flat surface!**

1. Place board on **flat, stable table**
2. **DO NOT MOVE** board during calibration
3. Press **RESET** button
4. Wait for "Calibrating..." message
5. Keep still for 3 seconds
6. Wait for "Calibration complete"

**Calibration requirements:**
- ❌ NO tilting
- ❌ NO vibration
- ❌ NO touching board
- ✅ Flat surface
- ✅ Completely still

#### ✅ Step 2: Check If Data Changes

**Test sensor responsiveness:**

1. Upload GetAllData.ino
2. Open Serial Monitor
3. **Tilt board** left/right
4. **Rotate board**
5. Watch if **ANY** value changes

**If NO change at all:**
- Sensor not reading
- Run I2C Scanner
- Check sensor power

**If some values change:**
- Re-calibrate properly
- Sensor is working!

#### ✅ Step 3: Check Expected Values

**When board is flat on table:**

| Sensor | Axis | Expected | Tolerance |
|--------|------|----------|-----------|
| Acc | X | ~0.0g | ±0.1g |
| Acc | Y | ~0.0g | ±0.1g |
| Acc | Z | ~1.0g | ±0.1g |
| Gyro | X | ~0°/s | ±5°/s |
| Gyro | Y | ~0°/s | ±5°/s |
| Gyro | Z | ~0°/s | ±5°/s |
| Angle | Roll | ~0° | ±5° |
| Angle | Pitch | ~0° | ±5° |

**If Acc Z ≠ 1g:**
- Board not flat
- Or calibration wrong
- Or sensor damaged

#### ✅ Step 4: Manual Offset Check

```cpp
// After calibration, print offsets:
Serial.print("Gyro X offset: ");
Serial.println(mpu.getGyroXoffset());
Serial.print("Acc X offset: ");
Serial.println(mpu.getAccXoffset());

// Offsets should be small numbers
// If offsets = 0.00 for all → calibration didn't run
// If offsets very large (>100) → sensor issue
```

---

## 6. Serial Monitor Menampilkan Sampah

### Shows:
```
ÿÿÿÿ���ÿÿ�ÿÿ�ÿÿÿÿ
⸮⸮⸮⸮⸮⸮⸮⸮⸮⸮
```

### Penyebab:
- Baud rate salah
- Board not in sync

### Solusi:

#### ✅ Step 1: Check Baud Rate

**Serial Monitor:**
- Bottom right dropdown
- Must be: **115200 baud**

**Code:**
```cpp
Serial.begin(115200);  // Must match!
```

#### ✅ Step 2: Restart Sequence

1. Close Serial Monitor
2. Press **RESET** button on board
3. Wait 2 seconds
4. Open Serial Monitor
5. Set baud = 115200

#### ✅ Step 3: Re-upload Code

Sometimes helps to clear buffers:

1. Close Serial Monitor
2. Click Upload
3. Wait "Done uploading"
4. Open Serial Monitor (115200)

---

## 7. Angles Tidak Berubah

### Data stuck:
```
Roll: 0.0°  Pitch: 0.0°  Yaw: 0.0°
Roll: 0.0°  Pitch: 0.0°  Yaw: 0.0°
```

### Atau angles frozen

### Solusi:

#### ✅ Check Update Function

```cpp
void loop() {
  mpu.update();  // ← MUST be called every loop!
  
  // Then get angles
  float roll = mpu.getAngleX();
}
```

#### ✅ Check Complementary Filter

```cpp
// Filter coefficient (0.0 - 1.0)
mpu.setFilterGyroCoef(0.98);  // 98% gyro, 2% accel

// If set to 0.0 or 1.0, angles may not update properly
// Default 0.98 is good for most cases
```

#### ✅ Check If Gyro Working

```cpp
// Print raw gyro
Serial.println(mpu.getGyroX());
Serial.println(mpu.getGyroY());
Serial.println(mpu.getGyroZ());

// Rotate board → values should change
// If gyro = 0 always → gyro not working
```

---

## 8. Yaw Drift Terus

### Yaw keeps increasing:
```
Yaw: 10.0°
Yaw: 15.2°
Yaw: 21.5°
Yaw: 28.1°  ← Keeps increasing even when board is still!
```

### **This is NORMAL!**

### Explanation:

**Gyroscope drift is inherent physics:**
- Gyro measures rotation *speed* (°/s)
- Yaw = integrated gyro over time
- Small errors accumulate
- No absolute reference for yaw

**Solutions:**

#### ✅ Option 1: Accept It

For short-term use (< 1 minute), drift is minimal.

#### ✅ Option 2: Reset Yaw Periodically

```cpp
void loop() {
  mpu.update();
  
  // Reset yaw every 10 seconds
  static unsigned long lastReset = 0;
  if (millis() - lastReset > 10000) {
    mpu.angleZ = 0;  // Reset yaw
    lastReset = millis();
  }
}
```

#### ✅ Option 3: Use Magnetometer

Add magnetometer (compass) for absolute heading:
- QMC5883L
- HMC5883L
- LSM303

#### ✅ Option 4: Kalman Filter

Implement Kalman filter for better fusion.

**Note:** Roll and Pitch don't drift because gravity provides absolute reference!

---

## 9. Calibration Gagal

### Shows:
```
Calibrating...
✅ Calibration complete!

Offsets:
Gyro X: 0.00  ← All zeros = BAD!
Gyro Y: 0.00
Gyro Z: 0.00
```

### Atau angles still wrong after calibration

### Solusi:

#### ✅ Keep Board PERFECTLY Still

**Calibration needs:**
- Flat, stable surface
- No movement for 3+ seconds
- No vibration
- No touching

**Try:**
1. Put board on **table** (not hand)
2. Don't touch anything
3. No fans, no vibration
4. Press RESET
5. Watch "Calibrating..." message
6. Wait 5 seconds
7. Check offsets are NOT all zeros

#### ✅ Manual Calibration

```cpp
void setup() {
  // ... init code ...
  
  // Manual calibration with more samples
  Serial.println("Calibrating...");
  delay(2000);  // Extra delay
  
  mpu.calcOffsets(true, true);  // Both gyro and accel
  
  // Verify offsets
  Serial.print("Gyro offsets: ");
  Serial.print(mpu.getGyroXoffset()); Serial.print(", ");
  Serial.print(mpu.getGyroYoffset()); Serial.print(", ");
  Serial.println(mpu.getGyroZoffset());
}
```

---

## 10. Data Noise/Tidak Stabil

### Values jumping wildly:
```
Roll: 5.2°
Roll: 12.8°  ← Jumping!
Roll: 3.1°
Roll: 18.5°
```

### Solusi:

#### ✅ Check Mechanical Stability

- Sensor firmly mounted?
- No loose connections?
- Minimize vibration

#### ✅ Adjust Filter Coefficient

```cpp
// More accel (smoother but slower response)
mpu.setFilterGyroCoef(0.90);  // 90% gyro, 10% accel

// More gyro (faster response but may amplify noise)
mpu.setFilterGyroCoef(0.99);  // 99% gyro, 1% accel

// Default (balanced)
mpu.setFilterGyroCoef(0.98);  // 98% gyro, 2% accel
```

#### ✅ Software Filtering

```cpp
// Simple moving average
#define FILTER_SIZE 5
float rollBuffer[FILTER_SIZE];
int filterIndex = 0;

void loop() {
  mpu.update();
  
  // Add to buffer
  rollBuffer[filterIndex] = mpu.getAngleX();
  filterIndex = (filterIndex + 1) % FILTER_SIZE;
  
  // Calculate average
  float rollFiltered = 0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    rollFiltered += rollBuffer[i];
  }
  rollFiltered /= FILTER_SIZE;
  
  Serial.println(rollFiltered);
}
```

---

## 🆘 Still Stuck?

If masalah masih belum solved:

### Diagnostic Checklist:

- [ ] Run I2C_Scanner_Diagnostic.ino → 0x6A detected?
- [ ] Upload GetAllData.ino → ANY data changes?
- [ ] Check baud rate = 115200
- [ ] Library installed correctly?
- [ ] Arduino IDE restarted?
- [ ] Correct board (ESP32 Dev Module)?
- [ ] Correct port selected?
- [ ] USB cable works for data?
- [ ] Calibration done properly?
- [ ] Board on flat surface?

### Get Help:

1. **Document your problem:**
   - Error messages (copy full text)
   - Serial Monitor output
   - What you tried
   - Board model

2. **Share diagnostic info:**
   - I2C Scanner output
   - GetAllData output
   - Photos of wiring (if external sensor)

3. **Ask for help:**
   - FishBot GitHub: https://github.com/fishros/fishbot_motion_control_microros
   - Arduino Forums: https://forum.arduino.cc
   - ESP32 Forums: https://www.esp32.com

---

**Good luck! 🍀**
