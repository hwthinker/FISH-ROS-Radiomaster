# FishBot LSM6DS3 IMU - Complete Guide

Complete guide untuk menggunakan sensor LSM6DS3 di FishBot menggunakan library MPU6050_light.

## 📋 Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Library Installation](#library-installation)
- [Wiring Connection](#wiring-connection)
- [Quick Start](#quick-start)
- [Available Programs](#available-programs)
- [Troubleshooting](#troubleshooting)
- [FAQ](#faq)

---

## 🔧 Hardware Requirements

| Item | Description |
|------|-------------|
| **Board** | ESP32 FishBot |
| **IMU Chip** | LSM6DS3 (soldered on board at address 0x6A) |
| **USB Cable** | USB-C or Micro-USB (depends on your board) |
| **Computer** | Windows, Mac, or Linux |

**Note:** Your FishBot already has LSM6DS3 chip built-in. No external wiring needed!

---

## 💻 Software Requirements

### 1. Arduino IDE

**Download:** https://www.arduino.cc/en/software

**Supported versions:**
- Arduino IDE 1.8.19 or newer
- Arduino IDE 2.x (recommended)

### 2. ESP32 Board Support

Already installed? Skip to [Library Installation](#library-installation)

**Steps:**

1. **Open Arduino IDE**

2. **Go to:** File → Preferences

3. **Add ESP32 board URL:**
   
   In "Additional Boards Manager URLs", paste:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```

4. **Install ESP32 boards:**
   - Go to: Tools → Board → Boards Manager
   - Search: "esp32"
   - Install: "esp32 by Espressif Systems"
   - Wait for installation to complete

5. **Verify installation:**
   - Go to: Tools → Board
   - You should see "ESP32 Arduino" section

---

## 📚 Library Installation

### Method 1: Manual Installation (Recommended)

**Step 1: Create library folder**

Navigate to your Arduino libraries folder:

**Windows:**
```
C:\Users\[YourUsername]\Documents\Arduino\libraries\
```

**Mac:**
```
/Users/[YourUsername]/Documents/Arduino/libraries/
```

**Linux:**
```
/home/[YourUsername]/Arduino/libraries/
```

Create a new folder: `MPU6050_light`

**Step 2: Copy library files**

Copy these two files into the `MPU6050_light` folder:
- `MPU6050_light.h`
- `MPU6050_light.cpp`

**Final structure should be:**
```
Arduino/
└── libraries/
    └── MPU6050_light/
        ├── MPU6050_light.h
        └── MPU6050_light.cpp
```

**Step 3: Restart Arduino IDE**

⚠️ **IMPORTANT:** You MUST restart Arduino IDE for library to be recognized!

**Step 4: Verify installation**

- Open Arduino IDE
- Go to: Sketch → Include Library
- Look for "MPU6050_light" in the list
- ✅ If you see it, installation successful!

---

### Method 2: Install from ZIP

**Step 1: Create ZIP file**

Create a ZIP file containing:
```
MPU6050_light.zip
├── MPU6050_light.h
└── MPU6050_light.cpp
```

**Step 2: Install via Arduino IDE**

1. Open Arduino IDE
2. Go to: Sketch → Include Library → Add .ZIP Library
3. Select your `MPU6050_light.zip`
4. Click "Open"
5. Wait for "Library added to your libraries" message

**Step 3: Restart Arduino IDE**

**Step 4: Verify**

- Check: Sketch → Include Library
- Look for "MPU6050_light"

---

## 🔌 Wiring Connection

**Good news!** Your FishBot has LSM6DS3 already soldered on board!

### Internal Connections (Already Done)

| LSM6DS3 Pin | ESP32 Pin | Description |
|-------------|-----------|-------------|
| VCC | 3.3V | Power (DO NOT use 5V!) |
| GND | GND | Ground |
| SDA | GPIO 18 | I2C Data |
| SCL | GPIO 19 | I2C Clock |

**No external wiring needed!** Just connect USB cable to upload code.

---

## 🚀 Quick Start

### Step 1: Select Board

1. **Connect** FishBot to computer via USB
2. **Open** Arduino IDE
3. **Select Board:**
   - Go to: Tools → Board → ESP32 Arduino
   - Select: "ESP32 Dev Module"

4. **Select Port:**
   - Go to: Tools → Port
   - Select: COM port where FishBot is connected
   - Windows: COM3, COM4, etc.
   - Mac/Linux: /dev/ttyUSB0, /dev/cu.usbserial, etc.

### Step 2: Upload Test Program

1. **Open** example: `FishBot_LSM6DS3_GetAngle.ino`

2. **Click** Upload button (→)

3. **Wait** for compilation and upload
   - "Connecting..." → Press BOOT button if needed
   - "Writing..." → Upload in progress
   - "Done uploading" → Success!

### Step 3: View Output

1. **Open** Serial Monitor
   - Click: Tools → Serial Monitor
   - Or press: Ctrl+Shift+M (Windows) / Cmd+Shift+M (Mac)

2. **Set baud rate:** 115200

3. **Expected output:**
   ```
   ╔════════════════════════════════════════════╗
   ║  FishBot IMU - LSM6DS3 Angle Reader       ║
   ╚════════════════════════════════════════════╝

   Initializing LSM6DS3...
   ✅ LSM6DS3 connected!

   Calibrating... Keep sensor STILL!
   ✅ Calibration done!

   Roll: 0.5°	Pitch: -1.2°	Yaw: 0.0°
   Roll: 0.6°	Pitch: -1.1°	Yaw: 0.1°
   ```

4. **Test sensor:**
   - Tilt board left/right → Roll changes
   - Tilt board forward/back → Pitch changes
   - Rotate board → Yaw changes

### Step 4: Success! 🎉

If you see angles updating, your setup is complete!

---

## 📁 Available Programs

### 1. FishBot_LSM6DS3_GetAngle.ino

**Best for:** Real-time monitoring, testing movements

**Features:**
- ✅ Compact single-line output
- ✅ Fast updates (100 Hz)
- ✅ Shows Roll, Pitch, Yaw
- ✅ Easy to read

**Output:**
```
Roll: 9.9°	Pitch: 21.8°	Yaw: 19.1°
```

**When to use:**
- Testing robot movements
- Monitoring angles in real-time
- Quick sensor verification
- Connecting to data plotting tools

**Upload:**
```
File: FishBot_LSM6DS3_GetAngle.ino
Baud: 115200
Update: 100 Hz (10 ms)
```

---

### 2. FishBot_LSM6DS3_GetAllData.ino

**Best for:** Debugging, detailed analysis

**Features:**
- ✅ Shows ALL sensor data
- ✅ Temperature reading
- ✅ Accelerometer (X, Y, Z)
- ✅ Gyroscope (X, Y, Z)
- ✅ Angles (Roll, Pitch, Yaw)
- ✅ Beautiful formatting

**Output:**
```
┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
┃              LSM6DS3 SENSOR DATA                ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛

🌡️  TEMPERATURE: 22.9 °C

📐 ACCELEROMETER (g):
   X: -0.479   Y:  0.216   Z:  1.150

🔄 GYROSCOPE (°/s):
   X:    1.19   Y:   -5.58   Z:   -1.28

📊 ACC ANGLES (from accel only) (°):
   X:    9.83   Y:   22.27

🎯 FILTERED ANGLES (gyro + accel fusion) (°):
   X (Roll):     9.90   
   Y (Pitch):   21.78   
   Z (Yaw):     19.05
```

**When to use:**
- Verifying sensor calibration
- Debugging sensor issues
- Checking all readings at once
- Data logging for analysis
- Understanding sensor behavior

**Upload:**
```
File: FishBot_LSM6DS3_GetAllData.ino
Baud: 115200
Update: 1 Hz (1000 ms)
```

---

### 3. FishBot_LSM6DS3_Template.ino

**Best for:** Integrating IMU into your robot code

**Features:**
- ✅ Ready-to-use functions
- ✅ Non-blocking updates
- ✅ Fall detection built-in
- ✅ Tilt safety monitoring
- ✅ Easy-to-use getters

**Code example:**
```cpp
void setup() {
  // Initialize IMU
  if (!imuSetup()) {
    Serial.println("IMU failed!");
    while(1);
  }
  
  // Your robot setup...
  setupMotors();
}

void loop() {
  // Update IMU (non-blocking)
  imuUpdate();
  
  // Fall detection
  if (isFalling()) {
    emergencyStop();
  }
  
  // Get current angles
  float pitch = getPitch();
  float roll = getRoll();
  
  // Use for balance control
  balanceControl(pitch, roll);
}
```

**When to use:**
- Building robot control code
- Implementing balance control
- Adding fall detection
- Integrating with motor control
- Creating autonomous behaviors

**Upload:**
```
File: FishBot_LSM6DS3_Template.ino
Baud: 115200
Update: 100 Hz (configurable)
```

---

## 🔧 Step-by-Step Upload Guide

### Detailed Upload Process

**1. Prepare Arduino IDE**
```
✓ Library installed
✓ ESP32 board support installed
✓ Arduino IDE restarted
```

**2. Open Program**
```
File → Open → Select .ino file
Example: FishBot_LSM6DS3_GetAngle.ino
```

**3. Configure Board Settings**

Go to: Tools menu

| Setting | Value |
|---------|-------|
| Board | ESP32 Dev Module |
| Upload Speed | 921600 |
| CPU Frequency | 240MHz |
| Flash Frequency | 80MHz |
| Flash Mode | QIO |
| Flash Size | 4MB |
| Partition Scheme | Default |
| Core Debug Level | None |
| **Port** | Your COM port |

**4. Verify Code**

Click: ✓ (Verify button)

Wait for: "Done compiling"

**5. Upload Code**

Click: → (Upload button)

**If stuck at "Connecting...":**
- Hold BOOT button on FishBot
- Click Upload
- Release BOOT when "Writing..." appears

**6. Check Upload Success**

Look for:
```
Writing at 0x00001000... (100%)
Wrote XXXXX bytes
Hash of data verified.

Leaving...
Hard resetting via RTS pin...
```

**7. Open Serial Monitor**

- Click: Tools → Serial Monitor
- Set: Baud rate = 115200
- Set: Line ending = Newline (optional)

**8. Observe Output**

Press RESET button on FishBot if no output appears.

---

## 🐛 Troubleshooting

### Issue 1: Library Not Found

**Error message:**
```
MPU6050_light.h: No such file or directory
```

**Solutions:**

✅ **Check library location:**
```
Arduino/libraries/MPU6050_light/
├── MPU6050_light.h
└── MPU6050_light.cpp
```

✅ **Restart Arduino IDE** (very important!)

✅ **Verify installation:**
- Sketch → Include Library
- Look for "MPU6050_light"

✅ **Re-install if needed:**
- Delete old folder
- Copy files again
- Restart IDE

---

### Issue 2: COM Port Not Found

**Error message:**
```
Serial port not found
```

**Solutions:**

✅ **Windows: Install CP210x driver**
- Download from: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
- Install and restart

✅ **Check USB cable:**
- Use data cable (not charging-only cable)
- Try different USB port
- Try different cable

✅ **Check Device Manager (Windows):**
- Open: Device Manager
- Look under: Ports (COM & LPT)
- Should see: Silicon Labs CP210x USB to UART Bridge

✅ **Linux: Add user to dialout group:**
```bash
sudo usermod -a -G dialout $USER
# Log out and log back in
```

---

### Issue 3: Upload Failed

**Error message:**
```
Failed to connect to ESP32
A fatal error occurred
```

**Solutions:**

✅ **Hold BOOT button:**
1. Click Upload in Arduino IDE
2. When "Connecting..." appears
3. Hold BOOT button on FishBot
4. Release when "Writing..." appears

✅ **Press RESET after upload:**
- Some boards need manual reset
- Press RESET button once after upload complete

✅ **Try lower upload speed:**
- Tools → Upload Speed → 115200

✅ **Check USB connection:**
- Reconnect USB cable
- Try different USB port
- Check cable quality

---

### Issue 4: IMU Initialization Failed

**Serial Monitor shows:**
```
IMU Status: 1
❌ Failed to initialize LSM6DS3!
```

**Solutions:**

✅ **Run I2C Scanner first:**
```cpp
// Upload I2C_Scanner_Diagnostic.ino
// Check if 0x6A appears
```

✅ **Check I2C pins:**
```cpp
// Verify in code:
#define SDA_PIN 18
#define SCL_PIN 19
```

✅ **Power cycle:**
- Unplug USB
- Wait 5 seconds
- Plug back in
- Upload again

✅ **Check sensor power:**
- Sensor should have LED ON
- Measure voltage: VCC = 3.3V

---

### Issue 5: Wrong Data / All Zeros

**Serial Monitor shows:**
```
Acc: 0.000g  0.000g  0.000g
Gyro: 0.0°/s  0.0°/s  0.0°/s
```

**Solutions:**

✅ **Re-calibrate:**
- Press RESET button
- Keep board STILL during calibration
- Wait for "Calibration complete"

✅ **Check sensor communication:**
- Run GetAllData.ino
- Check if ANY data changes
- If all stuck at 0 → sensor not responding

✅ **Check register configuration:**
- Upload diagnostic script
- Verify WHO_AM_I = 0x6A

---

### Issue 6: Serial Monitor Shows Garbage

**Serial Monitor shows:**
```
ÿÿ���ÿ�ÿÿ�ÿ
```

**Solutions:**

✅ **Check baud rate:**
- Must be: 115200
- Change in Serial Monitor dropdown

✅ **Press RESET:**
- Close Serial Monitor
- Press RESET on board
- Open Serial Monitor

✅ **Re-upload code:**
- Sometimes helps to clear buffer

---

## ❓ FAQ

### Q1: Do I need to buy a separate IMU module?

**A:** No! Your FishBot already has LSM6DS3 chip built-in at address 0x6A. No external module needed.

---

### Q2: Can I use Adafruit_MPU6050 library?

**A:** No. That library only supports MPU6050 chip (address 0x68). Your board has LSM6DS3 (address 0x6A). Use MPU6050_light library instead, which supports both chips.

---

### Q3: Why is my board labeled "MPU6050" but it's actually LSM6DS3?

**A:** Marketing! Many Chinese boards write "MPU6050 compatible" but use cheaper alternative chips. Function is the same (6-axis IMU), but protocol is different.

---

### Q4: What's the difference between GetAngle and GetAllData?

**A:** Both read the same sensor data. GetAngle shows angles only at 100 Hz. GetAllData shows everything at 1 Hz. [See detailed comparison](Comparison_GetAllData_vs_GetAngle.md)

---

### Q5: Do I need to calibrate every time?

**A:** Yes! Run calibration (`mpu.calcOffsets()`) every time you power on. Keep robot STILL during calibration (takes ~3 seconds).

---

### Q6: Why does Yaw angle drift?

**A:** This is normal! Gyroscope has no absolute reference for yaw. Small errors accumulate over time. Solutions:
- Reset yaw periodically
- Use magnetometer for absolute heading
- Implement Kalman filter

---

### Q7: Can I use this with ROS?

**A:** Yes! Use the Template.ino as base. Publish data to `sensor_msgs/Imu` topic. FishBot repo may have ROS integration already.

---

### Q8: What's the maximum update rate?

**A:** Library can handle 1000 Hz, but 100 Hz is recommended for stability. Serial Monitor limits practical rate to ~100 Hz anyway.

---

### Q9: Can I read raw sensor values?

**A:** Yes! Use `mpu.getAccX()`, `mpu.getGyroX()`, etc. These give calibrated values in g and °/s.

---

### Q10: How accurate is the sensor?

**A:** LSM6DS3 typical specs:
- Accelerometer: ±0.05g accuracy
- Gyroscope: ±1°/s accuracy
- Temperature: ±1°C accuracy
- Good enough for most robot applications!

---

## 📞 Support

### If You're Still Stuck:

1. **Check documentation:**
   - [Why_Original_Failed_Analysis.md](Why_Original_Failed_Analysis.md)
   - [LSM6DS3_Clean_Version_Summary.md](LSM6DS3_Clean_Version_Summary.md)
   - [Comparison_GetAllData_vs_GetAngle.md](Comparison_GetAllData_vs_GetAngle.md)

2. **Run diagnostics:**
   - Upload: I2C_Scanner_Diagnostic.ino
   - Check if 0x6A detected
   - Share serial output

3. **Common fixes:**
   - Restart Arduino IDE
   - Re-install library
   - Try different USB cable
   - Press BOOT button during upload
   - Check baud rate = 115200

4. **Get help:**
   - FishBot official repo: https://github.com/fishros/fishbot_motion_control_microros
   - Arduino forums: https://forum.arduino.cc
   - ESP32 forums: https://www.esp32.com

---

## 📝 Quick Reference Card

### Essential Info

| Item | Value |
|------|-------|
| **Chip** | LSM6DS3 |
| **I2C Address** | 0x6A |
| **I2C Pins** | SDA=GPIO18, SCL=GPIO19 |
| **Library** | MPU6050_light |
| **Baud Rate** | 115200 |
| **Board** | ESP32 Dev Module |

### Upload Checklist

- [ ] Library installed
- [ ] ESP32 board support installed
- [ ] Arduino IDE restarted
- [ ] Board = ESP32 Dev Module
- [ ] Port selected
- [ ] Baud = 115200
- [ ] Code uploaded successfully
- [ ] Serial Monitor opened
- [ ] Data appearing

### Calibration Checklist

- [ ] Board on flat surface
- [ ] Board not moving
- [ ] No vibration
- [ ] Wait 3 seconds
- [ ] "Calibration complete" message

### Good Readings Checklist

- [ ] Acc Z ≈ 1.0g (when flat)
- [ ] Acc X, Y ≈ 0.0g (when flat)
- [ ] Gyro ≈ 0°/s (when still)
- [ ] Angles change when tilted
- [ ] No all-zero readings
- [ ] Temperature reasonable (20-30°C)

---

## 🎉 Success Criteria

You've successfully set up LSM6DS3 when:

✅ Upload completes without errors  
✅ Serial Monitor shows sensor data  
✅ Angles change when you tilt board  
✅ Temperature reading is reasonable  
✅ No error messages in Serial Monitor  
✅ Data updates continuously  

**Congratulations! Your FishBot IMU is working!** 🚀

---

## 📄 License

MIT License - Free to use and modify

---

## 👥 Credits

- **MPU6050_light Library:** Romain JL. Fétick, Tockn
- **FishBot Hardware:** FishROS Team
- **Documentation:** This guide

---

**Last Updated:** 2025-01-30

**Version:** 1.0

**Tested on:** 
- Arduino IDE 1.8.19, 2.3.2
- ESP32 Arduino Core 2.0.x, 3.0.x
- FishBot v2.x

---

*Happy Coding! 🤖*
