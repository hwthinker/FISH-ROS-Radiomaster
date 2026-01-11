# Laporan Analisis GPIO Mapping - FishBot Motion Control

## Executive Summary

Analisis berhasil mengekstrak **GPIO mapping lengkap** dari board FishBot Motion Control MicroROS v1.8.0. Total **12 GPIO pins** teridentifikasi dengan fungsi yang jelas dari analisis kode firmware.

---

## 🎯 Temuan Utama

### ✅ GPIO Teridentifikasi dengan Jelas (12 pins)

| No | GPIO | Fungsi | Status |
|----|------|--------|--------|
| 1 | 18 | I2C SDA (IMU + OLED) | ✅ Aktif |
| 2 | 19 | I2C SCL (IMU + OLED) | ✅ Aktif |
| 3 | 21 | Ultrasonic ECHO | ✅ Aktif |
| 4 | 25 | Encoder 1 B (Motor Kiri) | ✅ Aktif |
| 5 | 26 | Encoder 1 A (Motor Kiri) | ✅ Aktif |
| 6 | 27 | Ultrasonic TRIG | ✅ Aktif |
| 7 | 12 | Motor 1 A (Kiri Forward) | ✅ Aktif |
| 8 | 13 | Motor 1 B (Kiri Backward) | ✅ Aktif |
| 9 | 22 | Motor 0 A (Kanan Forward) | ✅ Aktif |
| 10 | 23 | Motor 0 B (Kanan Backward) | ✅ Aktif |
| 11 | 32 | Encoder 0 A (Motor Kanan) | ✅ Aktif |
| 12 | 33 | Encoder 0 B (Motor Kanan) | ✅ Aktif |

---

## 🔍 Analisis Driver IC pada Board

### 1. **DRV8833 - Dual H-Bridge Motor Driver** ✅ AKTIF

**Lokasi:** Terlihat jelas pada board image  
**Status:** **FULLY UTILIZED** - Digunakan untuk kontrol 2 motor DC

**Koneksi GPIO:**
```
Motor 0 (Kanan):
├── GPIO 22 → DRV8833 AIN1 (Forward)
└── GPIO 23 → DRV8833 AIN2 (Backward)

Motor 1 (Kiri):
├── GPIO 12 → DRV8833 BIN1 (Forward)
└── GPIO 13 → DRV8833 BIN2 (Backward)
```

**Spesifikasi:**
- Operating Voltage: 2.7V - 10.8V
- Output Current: 1.5A continuous (2A peak)
- PWM Control: Menggunakan ESP32 MCPWM peripheral
- Both channels: FULLY USED

**Kode Implementasi:**
```cpp
// Dari fishbot_config.h
#define CONFIG_DEFAULT_MOTOR0_A_GPIO 22
#define CONFIG_DEFAULT_MOTOR0_B_GPIO 23
#define CONFIG_DEFAULT_MOTOR1_A_GPIO 12
#define CONFIG_DEFAULT_MOTOR1_B_GPIO 13

// Dari fishbot.cpp
motor.attachMotors(CONFIG_DEFAULT_MOTOR0_A_GPIO, 
                   CONFIG_DEFAULT_MOTOR0_B_GPIO,
                   CONFIG_DEFAULT_MOTOR1_A_GPIO, 
                   CONFIG_DEFAULT_MOTOR1_B_GPIO);
```

---

### 2. **ULN2003A - Darlington Arrays (2 chips)** ⚠️ RESERVED

**Lokasi:** Teridentifikasi pada board (Anda menyebutkan ada 2 buah)  
**Status:** **NOT USED IN CURRENT FIRMWARE**

**Analisis:**
✅ Terdeteksi secara fisik di board (2 chips)  
❌ TIDAK ADA kode yang menggunakan ULN2003A  
❌ TIDAK ADA definisi GPIO untuk stepper motor  
❌ TIDAK ADA library stepper motor di firmware

**Pencarian Kode:**
```
Keywords searched: STEPPER, STEP_PIN, DIR_PIN, ULN2003
Results: TIDAK DITEMUKAN dalam codebase
```

**Kesimpulan:**
ULN2003A adalah **reserved hardware untuk future expansion**. Kemungkinan besar didesain untuk:
1. Kontrol stepper motor (28BYJ-48)
2. Ekspansi relay modules
3. High-current peripheral control
4. Optional features yang belum diimplementasi

**Rekomendasi Jika Ingin Menggunakan:**
Jika Anda ingin menggunakan ULN2003A untuk stepper motor:
```
GPIO yang direkomendasikan (FREE):
- GPIO 4, 5, 14, 16 (4 pins untuk stepper 4-phase)
- Pastikan tidak konflik dengan pin yang sudah digunakan
```

---

## 📊 Peripheral Utilization

### ESP32 Peripherals yang Digunakan:

| Peripheral | GPIO Used | Function | Status |
|-----------|-----------|----------|--------|
| **I2C** | 18, 19 | IMU + OLED (shared bus) | ✅ Active |
| **MCPWM** | 22, 23, 12, 13 | 2 Motor PWM control | ✅ Active |
| **PCNT** | 32, 33, 26, 25 | 2 Encoder quadrature | ✅ Active |
| **GPIO** | 27, 21 | Ultrasonic TRIG/ECHO | ✅ Active |

### Efisiensi Penggunaan:
- **I2C Bus:** ✅ Efficient - 1 bus untuk 2 device (shared)
- **MCPWM:** ✅ Optimal - 2 unit untuk 2 motor
- **PCNT:** ✅ Optimal - 2 unit untuk 2 encoder
- **GPIO:** ✅ Standard - 2 GPIO untuk ultrasonic

---

## 🔌 Konektivitas Komponen

### Komponen Terhubung dan Aktif:

1. **MPU6050 IMU**
   - Address: 0x68
   - GPIO: 18 (SDA), 19 (SCL)
   - Status: ✅ Publishing `/imu` topic

2. **SSD1306 OLED Display**
   - Address: 0x3C
   - GPIO: 18 (SDA), 19 (SCL)
   - Status: ✅ Displaying robot status

3. **HC-SR04 Ultrasonic**
   - GPIO: 27 (TRIG), 21 (ECHO)
   - Status: ✅ Publishing `/ultrasonic` topic

4. **2x DC Motors with Encoders**
   - Motor Control: GPIO 22, 23, 12, 13
   - Encoder Feedback: GPIO 32, 33, 26, 25
   - Status: ✅ Closed-loop control with PID

5. **DRV8833 Motor Driver**
   - Control: 4 GPIO (2 per motor)
   - Status: ✅ Full H-bridge control

---

## 💡 GPIO yang Masih Available untuk Ekspansi

### Free GPIOs:

**General Purpose I/O:**
- GPIO 4, 5, 14, 16, 17

**Input Only (ADC):**
- GPIO 34, 35, 36, 39

**⚠️ Hindari (Strapping pins):**
- GPIO 0, 2, 15 (akan mempengaruhi boot mode)

### Skenario Penggunaan ULN2003A:

Jika ingin mengaktifkan stepper motor dengan ULN2003A:

```cpp
// Contoh GPIO assignment untuk stepper motor
#define STEPPER_IN1  4
#define STEPPER_IN2  5
#define STEPPER_IN3  14
#define STEPPER_IN4  16

// Butuh implementasi:
// 1. Stepper library (misal: AccelStepper)
// 2. GPIO configuration
// 3. Step sequence control
```

---

## 📝 Analisis Kode dari CSV

### Files yang Dianalisis:
1. `fishbot_config.h` - Definisi GPIO constants
2. `fishbot.cpp` - Implementasi motor, encoder, IMU
3. `example_main_backup.cpp` - Test/example code
4. `fishbot.h` - Header includes dan declarations

### GPIO Definitions Found:

#### I2C Configuration:
```cpp
Line 52-53: IMU I2C pins
#define CONFIG_DEFAULT_IMU_SDA_GPIO 18
#define CONFIG_DEFAULT_IMU_SCL_GPIO 19

Line 55-56: OLED I2C pins (sama dengan IMU - shared bus)
#define CONFIG_DEFAULT_OLED_SDA_GPIO 18
#define CONFIG_DEFAULT_OLED_SCL_GPIO 19
```

#### Ultrasonic Configuration:
```cpp
Line 60-61: Ultrasonic sensor pins
#define CONFIG_DEFAULT_ULTRASONIC_TRIG_GPIO 27
#define CONFIG_DEFAULT_ULTRASONIC_ECHO_GPIO 21
```

#### Encoder Configuration:
```cpp
Line 70-73: Encoder pins
#define CONFIG_DEFAULT_ENCODER0_A_GPIO 32  // Motor Kanan
#define CONFIG_DEFAULT_ENCODER0_B_GPIO 33
#define CONFIG_DEFAULT_ENCODER1_A_GPIO 26  // Motor Kiri
#define CONFIG_DEFAULT_ENCODER1_B_GPIO 25
```

#### Motor Control Configuration:
```cpp
Line 77-80: Motor driver pins
#define CONFIG_DEFAULT_MOTOR0_A_GPIO 22  // Motor Kanan
#define CONFIG_DEFAULT_MOTOR0_B_GPIO 23
#define CONFIG_DEFAULT_MOTOR1_A_GPIO 12  // Motor Kiri
#define CONFIG_DEFAULT_MOTOR1_B_GPIO 13
```

### Motor Parameters Found:
```cpp
Reduction Ratio: 40.5:1
Pulse per Revolution: 44
Wheel Diameter: 65mm

PID Controller:
Kp = 0.625
Ki = 0.125
Kd = 0.0
Output Limit = [-100, 100]
```

---

## 🎛️ Control Architecture Analysis

### Closed-Loop Control Flow:

```
ROS2 /cmd_vel → Kinematics → Target Speed → PID Controller
                                              ↓
                                         PWM Output
                                              ↓
                                    DRV8833 → DC Motor
                                              ↓
                                    Encoder (PCNT)
                                              ↓
                                      Actual Speed
                                              ↓
                                    Back to PID (Feedback)
                                              ↓
                                    Publish /odom
```

**Karakteristik:**
- ✅ Closed-loop velocity control
- ✅ Hardware quadrature decoding (PCNT)
- ✅ Hardware PWM generation (MCPWM)
- ✅ Real-time feedback dari encoder
- ✅ Odometry calculation dan publishing

---

## 🚦 Rekomendasi

### 1. **Untuk Penggunaan Existing Features:**
✅ Semua GPIO sudah terdefinisi dengan baik  
✅ Kode firmware sudah lengkap dan siap pakai  
✅ Tidak ada pin conflict  
✅ Peripheral allocation sudah optimal

### 2. **Untuk Ekspansi dengan ULN2003A:**

**Jika ingin menambahkan stepper motor:**

```cpp
// Tambahkan ke fishbot_config.h
#define CONFIG_STEPPER_IN1_GPIO 4
#define CONFIG_STEPPER_IN2_GPIO 5
#define CONFIG_STEPPER_IN3_GPIO 14
#define CONFIG_STEPPER_IN4_GPIO 16

// Tambahkan library
#include <Stepper.h>
// atau
#include <AccelStepper.h>

// Implementasi stepper control
void initStepper() {
    pinMode(CONFIG_STEPPER_IN1_GPIO, OUTPUT);
    pinMode(CONFIG_STEPPER_IN2_GPIO, OUTPUT);
    pinMode(CONFIG_STEPPER_IN3_GPIO, OUTPUT);
    pinMode(CONFIG_STEPPER_IN4_GPIO, OUTPUT);
}
```

### 3. **Peripheral yang Masih Free:**

| Peripheral | Available Units | Suggested Use |
|-----------|----------------|---------------|
| ADC | Multiple channels | Battery monitoring, sensors |
| SPI | VSPI, HSPI | External flash, display |
| UART | UART1, UART2 | GPS, additional sensors |
| Touch | 10 pins | Touch buttons, capacitive sensing |
| DAC | 2 channels | Audio output, analog control |

### 4. **Best Practices:**

✅ **Jangan ubah GPIO yang sudah terpakai** kecuali benar-benar perlu  
✅ **Test secara bertahap** jika menambah peripheral baru  
✅ **Dokumentasi perubahan** di konfigurasi file  
✅ **Hindari strapping pins** (GPIO 0, 2, 15) untuk peripheral normal  
✅ **Perhatikan current rating** jika menggunakan ULN2003A

---

## 📁 Deliverables

Dokumentasi lengkap telah dibuat:

1. **FishBot_GPIO_Mapping.md**
   - Dokumentasi lengkap GPIO mapping
   - Spesifikasi hardware detail
   - Control flow diagram
   - ROS2 topics information

2. **FishBot_GPIO_Mapping.xlsx**
   - 5 sheets dengan informasi terstruktur:
     * GPIO Mapping (pin assignment)
     * Motor System (motor & encoder config)
     * Sensors (I2C & ultrasonic details)
     * Driver ICs (DRV8833 & ULN2003A specs)
     * Quick Reference (code defs & ROS topics)

3. **FishBot_GPIO_Visual_Map.txt**
   - ASCII art diagram visual
   - Quick reference tables
   - Control flow diagram
   - Expansion GPIO suggestions

4. **Laporan Analisis (File ini)**
   - Temuan lengkap
   - Analisis driver IC
   - Rekomendasi ekspansi

---

## ✅ Kesimpulan

### Hasil Ekstraksi:
- ✅ **12 GPIO pins** berhasil diidentifikasi dengan lengkap
- ✅ **1 DRV8833** fully utilized untuk 2 DC motor
- ✅ **2 ULN2003A** ada di board tapi NOT USED (reserved)
- ✅ **Semua peripheral** (I2C, MCPWM, PCNT) teridentifikasi
- ✅ **Closed-loop control** dengan encoder feedback aktif
- ✅ **ROS2 integration** lengkap dengan topics

### Ketersediaan untuk Ekspansi:
- ✅ Multiple GPIO masih free (4, 5, 14, 16, 17, 34-39)
- ✅ ULN2003A dapat diaktifkan untuk stepper/relay
- ✅ ADC, SPI, UART peripherals masih available
- ✅ Dokumentasi lengkap untuk development

### Next Steps (Opsional):
1. **Jika ingin test existing features:**
   - Upload firmware ke ESP32
   - Test motor control dengan ROS2
   - Verifikasi sensor readings

2. **Jika ingin ekspansi dengan ULN2003A:**
   - Pilih GPIO yang free
   - Tambahkan stepper library
   - Implementasi step sequence control
   - Update dokumentasi

---

**Dibuat:** Oktober 2025  
**Source:** https://github.com/fishros/fishbot_motion_control_microros  
**Board Version:** v1.8.0  
**Analyst:** Hardware Analysis Tool
