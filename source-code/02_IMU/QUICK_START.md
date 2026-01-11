# 🚀 Quick Start Guide - FishBot LSM6DS3

Panduan super cepat untuk mulai menggunakan LSM6DS3 di FishBot.

---

## ⚡ 3 Langkah Cepat

### 1️⃣ Install Library (5 menit)

**Copy 2 file ini:**
- `MPU6050_light.h`
- `MPU6050_light.cpp`

**Ke folder ini:**
```
Windows: C:\Users\[Username]\Documents\Arduino\libraries\MPU6050_light\
Mac:     /Users/[Username]/Documents/Arduino/libraries/MPU6050_light/
Linux:   /home/[Username]/Arduino/libraries/MPU6050_light/
```

**⚠️ PENTING: Restart Arduino IDE!**

---

### 2️⃣ Upload Code (2 menit)

1. Buka: `FishBot_LSM6DS3_GetAngle.ino`
2. Pilih Board: **ESP32 Dev Module**
3. Pilih Port: Your COM port
4. Klik Upload (→)
5. Tekan BOOT button jika diminta

---

### 3️⃣ Lihat Hasil (1 menit)

1. Buka Serial Monitor (Ctrl+Shift+M)
2. Set baud: **115200**
3. Lihat data angles!

```
Roll: 9.9°	Pitch: 21.8°	Yaw: 19.1°
```

✅ **Done! IMU Working!**

---

## 📁 File Yang Mana?

### Untuk Monitoring Cepat:
👉 **`FishBot_LSM6DS3_GetAngle.ino`**
- Angles saja (Roll, Pitch, Yaw)
- Update 100x per detik
- Compact output

### Untuk Debug Detail:
👉 **`FishBot_LSM6DS3_GetAllData.ino`**
- Semua data sensor
- Update 1x per detik
- Formatted table

### Untuk Robot Code:
👉 **`FishBot_LSM6DS3_Template.ino`**
- Ready-to-use functions
- Fall detection
- Easy integration

---

## 🔧 Troubleshooting Cepat

### ❌ Library not found
```
Solution: Restart Arduino IDE!
```

### ❌ Port not found
```
Solution: Install CP210x driver
Windows: https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
```

### ❌ Upload failed
```
Solution: Hold BOOT button when "Connecting..." appears
```

### ❌ IMU not working
```
Solution: 
1. Run I2C_Scanner_Diagnostic.ino
2. Check if 0x6A detected
3. Re-calibrate (press RESET, keep board STILL)
```

---

## ✅ Check If Working

**Good readings:**
- ✅ Acc Z ≈ 1.0g (board flat)
- ✅ Angles change when tilt board
- ✅ Temperature 20-30°C
- ✅ No all-zero values

**Bad readings:**
- ❌ All zeros
- ❌ All 255 or -1
- ❌ No change when moving
- ❌ Error messages

---

## 📚 Full Documentation

Butuh detail lebih? Baca:
- **[README.md](README.md)** - Complete guide
- **[Comparison_GetAllData_vs_GetAngle.md](Comparison_GetAllData_vs_GetAngle.md)** - File comparison
- **[Why_Original_Failed_Analysis.md](Why_Original_Failed_Analysis.md)** - Why original code failed

---

## 🎯 Summary Checklist

Ikuti checklist ini:

**Installation:**
- [ ] Copy 2 files to libraries/MPU6050_light/
- [ ] Restart Arduino IDE
- [ ] Verify: Sketch → Include Library → See "MPU6050_light"

**Board Setup:**
- [ ] Connect FishBot via USB
- [ ] Tools → Board → ESP32 Dev Module
- [ ] Tools → Port → Select your COM port

**Upload:**
- [ ] Open .ino file
- [ ] Click Upload
- [ ] Hold BOOT if needed
- [ ] Wait "Done uploading"

**Test:**
- [ ] Open Serial Monitor
- [ ] Set baud = 115200
- [ ] See sensor data
- [ ] Tilt board → angles change

**✅ All checked? Success!** 🎉

---

## ⏱️ Total Time: ~10 menit

- Library install: 5 min
- Upload code: 2 min
- Test sensor: 1 min
- Celebrate: 2 min 🎊

---

**Need help?** Read full [README.md](README.md)

**Ready to code?** Use [FishBot_LSM6DS3_Template.ino](FishBot_LSM6DS3_Template.ino)

**Let's go!** 🚀
