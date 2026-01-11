# FishBot Motion Control - GPIO Mapping Documentation

## Board: FishBot Motion Control MicroROS v1.8.0
**Repository:** https://github.com/fishros/fishbot_motion_control_microros

---

## 📌 GPIO Pin Assignment Summary

| GPIO | Fungsi | Peripheral | Driver IC | Catatan |
|------|--------|-----------|-----------|---------|
| **2** | LED | LED |  | Internal LED di board |
| **18** | IMU SDA / OLED SDA | I2C (Shared) | - | Bus I2C bersama untuk LSM6DS3 dan SSD1306 |
| **19** | IMU SCL / OLED SCL | I2C (Shared) | - | Bus I2C bersama untuk LSM6DS3 dan SSD1306 |
| **21** | Ultrasonic ECHO | GPIO Input | - | HC-SR04 Echo pin |
| **25** | Encoder 1 B | PCNT Unit 1 | - | Motor Kiri Encoder - Channel B |
| **26** | Encoder 1 A | PCNT Unit 1 | - | Motor Kiri Encoder - Channel A |
| **27** | Ultrasonic TRIG | GPIO Output | - | HC-SR04 Trigger pin |
| **12** | Motor 1 A | MCPWM1A | DRV8833 | Motor Kiri Forward |
| **13** | Motor 1 B | MCPWM1B | DRV8833 | Motor Kiri Backward |
| **22** | Motor 0 A | MCPWM0A | DRV8833 | Motor Kanan Forward |
| **23** | Motor 0 B | MCPWM0B | DRV8833 | Motor Kanan Backward |
| **32** | Encoder 0 A | PCNT Unit 0 | - | Motor Kanan Encoder - Channel A |
| **33** | Encoder 0 B | PCNT Unit 0 | - | Motor Kanan Encoder - Channel B |

---

## 🔧 Komponen Hardware dan Driver IC

### 1. **DRV8833 - Dual H-Bridge Motor Driver** (1 chip)
Digunakan untuk mengontrol 2 motor DC dengan kontrol PWM menggunakan ESP32 MCPWM peripheral.

```
DRV8833 Connections:
┌─────────────────────────┐
│      DRV8833 Chip       │
├─────────────────────────┤
│ AIN1 ← GPIO 22 (MCPWM0A)│ → Motor 0 (Kanan) Forward
│ AIN2 ← GPIO 23 (MCPWM0B)│ → Motor 0 (Kanan) Backward
│ BIN1 ← GPIO 12 (MCPWM1A)│ → Motor 1 (Kiri) Forward
│ BIN2 ← GPIO 13 (MCPWM1B)│ → Motor 1 (Kiri) Backward
└─────────────────────────┘
```

**Karakteristik:**
- Voltage: 2.7V - 10.8V
- Current: 1.5A per channel (peak 2A)
- PWM Frequency: Menggunakan MCPWM ESP32
- Sleep Mode: Available

### 2. **ULN2003A - Darlington Transistor Arrays** (2 chips)
Terdeteksi di board tetapi **TIDAK digunakan dalam kode saat ini**.

**Kemungkinan Penggunaan:**
- Kontrol stepper motor (28BYJ-48)
- Relay atau beban induktif lainnya
- GPIO expansion untuk beban tinggi

⚠️ **Catatan:** Tidak ada definisi GPIO untuk stepper motor dalam kode firmware saat ini. ULN2003A kemungkinan untuk ekspansi future atau fitur opsional.

---

## 🎯 Peripheral Mapping Detail

### 📡 **I2C Bus (GPIO 18, 19)**

**Shared I2C Bus:**
```
I2C Bus
├── SDA: GPIO 18
└── SCL: GPIO 19
    ├── LSM6DS3 IMU (0x6A)
    └── SSD1306 OLED (0x3C)
```

**Konfigurasi Kode:**
```cpp
#define CONFIG_DEFAULT_IMU_SDA_GPIO 18
#define CONFIG_DEFAULT_IMU_SCL_GPIO 19
#define CONFIG_DEFAULT_OLED_SDA_GPIO 18
#define CONFIG_DEFAULT_OLED_SCL_GPIO 19
```

**Devices:**
1. **LSM6DS3 IMU Sensor**
   - 6-axis accelerometer + gyroscope
   - I2C Address: 0x6A
   - Publish: `sensor_msgs/Imu`
   
2. **SSD1306 OLED Display**
   - Resolution: 128x64 pixels
   - I2C Address: 0x3C
   - Display status robot

---

### 🏎️ **Motor Control System**

#### **A. DC Motor Control (DRV8833 + MCPWM)**

```
Motor 0 (Kanan/Right):
├── GPIO 22 (MCPWM0A) → DRV8833 AIN1 → Forward
├── GPIO 23 (MCPWM0B) → DRV8833 AIN2 → Backward
└── Control: H-Bridge PWM

Motor 1 (Kiri/Left):
├── GPIO 12 (MCPWM1A) → DRV8833 BIN1 → Forward
├── GPIO 13 (MCPWM1B) → DRV8833 BIN2 → Backward
└── Control: H-Bridge PWM
```

**Konfigurasi Kode:**
```cpp
#define CONFIG_DEFAULT_MOTOR0_A_GPIO 22
#define CONFIG_DEFAULT_MOTOR0_B_GPIO 23
#define CONFIG_DEFAULT_MOTOR1_A_GPIO 12
#define CONFIG_DEFAULT_MOTOR1_B_GPIO 13

motor.attachMotors(CONFIG_DEFAULT_MOTOR0_A_GPIO, 
                   CONFIG_DEFAULT_MOTOR0_B_GPIO,
                   CONFIG_DEFAULT_MOTOR1_A_GPIO, 
                   CONFIG_DEFAULT_MOTOR1_B_GPIO);
```

**Motor Parameters:**
- Reduction Ratio: 40.5:1
- Pulse per Revolution: 44
- Wheel Diameter: 65mm

#### **B. Motor Encoders (PCNT - Pulse Counter)**

```
Encoder 0 (Motor Kanan):
├── GPIO 32 → PCNT Unit 0 → Channel A
├── GPIO 33 → PCNT Unit 0 → Channel B
└── Quadrature Encoding

Encoder 1 (Motor Kiri):
├── GPIO 26 → PCNT Unit 1 → Channel A
├── GPIO 25 → PCNT Unit 1 → Channel B
└── Quadrature Encoding
```

**Konfigurasi Kode:**
```cpp
#define CONFIG_DEFAULT_ENCODER0_A_GPIO 32
#define CONFIG_DEFAULT_ENCODER0_B_GPIO 33
#define CONFIG_DEFAULT_ENCODER1_A_GPIO 26
#define CONFIG_DEFAULT_ENCODER1_B_GPIO 25

encoders[0].init(CONFIG_DEFAULT_ENCODER0_A_GPIO, 
                 CONFIG_DEFAULT_ENCODER0_B_GPIO, 
                 CONFIG_DEFAULT_PCNT_UTIL_00);
encoders[1].init(CONFIG_DEFAULT_ENCODER1_A_GPIO, 
                 CONFIG_DEFAULT_ENCODER1_B_GPIO, 
                 CONFIG_DEFAULT_PCNT_UTIL_01);
```

**Features:**
- Hardware quadrature decoding
- Direction detection
- Velocity calculation
- Odometry publishing

---

### 📏 **Ultrasonic Sensor (HC-SR04)**

```
HC-SR04 Ultrasonic Sensor:
├── TRIG: GPIO 27 (Output)
└── ECHO: GPIO 21 (Input)
```

**Konfigurasi Kode:**
```cpp
#define CONFIG_DEFAULT_ULTRASONIC_TRIG_GPIO 27
#define CONFIG_DEFAULT_ULTRASONIC_ECHO_GPIO 21
```

**Specifications:**
- Range: 2cm - 400cm
- Accuracy: ±3mm
- Trigger pulse: 10µs
- Publish: `sensor_msgs/Range`

---

## 🔌 Koneksi Board - Diagram Visual

```
┌──────────────────────────────────────────────────────────────────┐
│                    FishBot Motion Control Board                   │
├──────────────────────────────────────────────────────────────────┤
│                                                                   │
│  ┌────────┐      ┌──────────┐      ┌──────────┐                │
│  │ ESP32  │──I2C─┤ LSM6DS3  │      │ SSD1306  │                │
│  │        │ 18,19│   IMU    │      │   OLED   │                │
│  └────┬───┘      └──────────┘      └──────────┘                │
│       │                                                           │
│       ├─GPIO27,21─┤ HC-SR04 Ultrasonic ├─                      │
│       │                                                           │
│       │          ┌──────────────┐                                │
│       ├─22,23───┤   DRV8833    ├─── Motor 0 + Encoder 0        │
│       ├─12,13───┤  (H-Bridge)  ├─── Motor 1 + Encoder 1        │
│       │          └──────────────┘                                │
│       │                                                           │
│       ├─32,33─── Encoder 0 (PCNT0)                              │
│       └─26,25─── Encoder 1 (PCNT1)                              │
│                                                                   │
│  [ ULN2003A x2 ] ← Tidak digunakan (Reserved)                   │
│                                                                   │
└──────────────────────────────────────────────────────────────────┘
```

---

## 🎛️ Control Architecture

### **Motor Control Loop**

```
ROS2 cmd_vel
    ↓
Inverse Kinematics → Target Motor Speed (rad/s)
    ↓
PID Controller (Kp=0.625, Ki=0.125, Kd=0.0)
    ↓
PWM Output (-100 to +100)
    ↓
DRV8833 H-Bridge → Motor DC
    ↓
Encoder Feedback (PCNT) → Actual Speed
    ↓
Back to PID Controller (Closed Loop)
```

**PID Parameters:**
```cpp
Kp = 0.625
Ki = 0.125
Kd = 0.0
Output Limit = [-100, 100]
```

---

## 📊 ROS2 Topics

### **Publishers:**
- `/odom` (nav_msgs/Odometry) - Odometry dari encoder
- `/imu` (sensor_msgs/Imu) - Data IMU 6-axis
- `/ultrasonic` (sensor_msgs/Range) - Jarak dari ultrasonic

### **Subscribers:**
- `/cmd_vel` (geometry_msgs/Twist) - Command velocity untuk robot

---

## ⚡ Power System

Dari diagram board:
- **12V Input:** Battery/Power input untuk motor
- **5V Output:** Regulated 5V untuk ESP32 dan peripherals
- **USB Power:** USB Type-C untuk programming dan debugging
- **Power Switch:** Main power switch

---

## 🔍 GPIO Reserved / Tidak Digunakan

### **ULN2003A Driver Pins** (Reserved untuk future use)
Kemungkinan penggunaan:
- Stepper motor control (4-8 GPIO pins)
- Relay atau solenoid control
- High-current LED strips
- Servo expansion

**Perhatian:** Jika ingin menggunakan ULN2003A, pastikan GPIO yang dipilih tidak konflik dengan pin yang sudah digunakan.

---

## 🛠️ Development Notes

### **Peripheral Conflicts to Avoid:**
1. **I2C pins (18, 19):** Jangan gunakan untuk fungsi lain
2. **MCPWM pins (12, 13, 22, 23):** Reserved untuk motor PWM
3. **PCNT pins (25, 26, 32, 33):** Reserved untuk encoder
4. **Strapping pins:** GPIO 0, 2, 15 (hati-hati saat boot)

### **Available GPIOs for Expansion:**
Cek GPIO yang masih free di ESP32 untuk ekspansi:
- GPIO 4, 5, 14, 15, 16, 17, 34, 35, 36, 39 (Input only)

---

## 📚 Source Code References

**Main Configuration File:**
```
include/fishbot_config.h
```

**Key Classes:**
- `Esp32McpwmMotor` - Motor control dengan MCPWM
- `Esp32PcntEncoder` - Encoder reading dengan PCNT
- `ImuDriver` - LSM6DS3 IMU driver
- `OledDisplay` - SSD1306 OLED display
- `UltrasonicDriver` - HC-SR04 ultrasonic sensor

---

## ✅ Testing & Verification

### **GPIO Test Checklist:**

- [ ] I2C Communication (IMU + OLED): GPIO 18, 19
- [ ] Motor 0 PWM: GPIO 22, 23
- [ ] Motor 1 PWM: GPIO 12, 13
- [ ] Encoder 0: GPIO 32, 33
- [ ] Encoder 1: GPIO 26, 25
- [ ] Ultrasonic: GPIO 27, 21

### **Hardware Test:**
```bash
# Test motors
rostopic pub /cmd_vel geometry_msgs/Twist "linear: {x: 0.1}" -1

# Monitor encoders
rostopic echo /odom

# Check IMU
rostopic echo /imu

# Check ultrasonic
rostopic echo /ultrasonic
```

---

## 📝 Revision History

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2025 | Initial GPIO mapping documentation |

---

**Generated from:** FishBot Motion Control MicroROS Repository
**Board Version:** v1.8.0
**Documentation Date:** October 2025
