import serial
import time
import threading

PORT = "COM10"  # Ganti sesuai port kamu
BAUD = 420000

ser = serial.Serial(PORT, BAUD, timeout=0.01)

# ==========================================
# DECODE FUNCTIONS (Receive RC Channels)
# ==========================================

def decode_channels(payload):
    ch = []
    bit_buffer = 0
    bit_count = 0

    for b in payload:
        bit_buffer |= b << bit_count
        bit_count += 8
        while bit_count >= 11 and len(ch) < 16:
            ch.append(bit_buffer & 0x7FF)
            bit_buffer >>= 11
            bit_count -= 11
    return ch

def crsf_to_pwm(c):
    return int((c - 172) * 1000 / (1811 - 172) + 1000)

# ==========================================
# TELEMETRY FUNCTIONS (Send to RX2)
# ==========================================

def crc8_dvb_s2(data):
    """CRC8 untuk CRSF"""
    crc = 0
    for byte in data:
        crc ^= byte
        for _ in range(8):
            if crc & 0x80:
                crc = (crc << 1) ^ 0xD5
            else:
                crc = crc << 1
            crc &= 0xFF
    return crc

def send_battery_telemetry(voltage, current, capacity, percent):
    """Kirim Battery Sensor (0x08) - Format CRSF Standard"""
    # CRSF Telemetry Address (from FC to TX)
    dest_addr = 0xC8  # CRSF sync byte
    
    # Frame type
    frame_type = 0x08  # Battery Sensor
    
    # Pack data
    voltage_data = int(voltage * 10)   # dV
    current_data = int(current * 10)   # dA
    capacity_data = int(capacity)      # mAh
    percent_data = int(percent)        # %
    
    # Build payload
    payload = bytearray([
        frame_type,
        (voltage_data >> 8) & 0xFF,
        voltage_data & 0xFF,
        (current_data >> 8) & 0xFF,
        current_data & 0xFF,
        (capacity_data >> 16) & 0xFF,
        (capacity_data >> 8) & 0xFF,
        capacity_data & 0xFF,
        percent_data
    ])
    
    # Calculate length
    length = len(payload) + 1  # +1 for CRC
    
    # Build complete frame
    frame = bytearray([dest_addr, length])
    frame.extend(payload)
    frame.append(crc8_dvb_s2(payload))
    
    # Send
    ser.write(frame)
    return frame

def send_link_stats():
    """Kirim Link Statistics (0x14) - Ini yang paling sering tampil"""
    dest_addr = 0xC8
    frame_type = 0x14
    
    # Link stats data
    uplink_rssi_1 = 100          # RSSI ant 1
    uplink_rssi_2 = 100          # RSSI ant 2  
    uplink_link_quality = 100    # LQ 0-100%
    uplink_snr = 10              # SNR
    active_antenna = 0           # Active ant
    rf_mode = 2                  # RF mode
    uplink_tx_power = 14         # TX power
    downlink_rssi = 100          # Downlink RSSI
    downlink_link_quality = 100  # Downlink LQ
    downlink_snr = 10            # Downlink SNR
    
    payload = bytearray([
        frame_type,
        uplink_rssi_1,
        uplink_rssi_2,
        uplink_link_quality,
        uplink_snr,
        active_antenna,
        rf_mode,
        uplink_tx_power,
        downlink_rssi,
        downlink_link_quality,
        downlink_snr
    ])
    
    length = len(payload) + 1
    frame = bytearray([dest_addr, length])
    frame.extend(payload)
    frame.append(crc8_dvb_s2(payload))
    
    ser.write(frame)
    return frame

# ==========================================
# THREADS - Receive & Send Simultaneously
# ==========================================

rx_active = False
telemetry_sent = 0

def receive_thread():
    """Thread untuk terima RC channels dari RX2"""
    global rx_active
    print("[RX Thread] Started - receiving RC channels...")
    
    while True:
        try:
            if ser.read(1) != b'\xC8':
                continue

            length = ser.read(1)[0]
            frame = ser.read(length)
            
            if len(frame) < length:
                continue
                
            frame_type = frame[0]

            if frame_type == 0x16:  # RC Channels
                rx_active = True
                payload = frame[1:-1]
                ch_raw = decode_channels(payload)
                pwm = [crsf_to_pwm(c) for c in ch_raw]
                
                print(f"[RX] THR:{pwm[2]:4} ROLL:{pwm[0]:4} PITCH:{pwm[1]:4} YAW:{pwm[3]:4}")
        except:
            pass

def telemetry_thread():
    """Thread untuk kirim telemetry ke RX2"""
    global telemetry_sent
    print("[TX Thread] Started - sending telemetry...")
    time.sleep(2)  # Tunggu dulu RX stabil
    
    counter = 0
    while True:
        try:
            counter += 1
            
            # Kirim Battery
            send_battery_telemetry(
                voltage=12.6,
                current=5.2,
                capacity=850,
                percent=75
            )
            time.sleep(0.05)
            
            # Kirim Link Stats
            send_link_stats()
            
            telemetry_sent += 1
            
            if counter % 10 == 0:
                print(f"[TX] Sent {telemetry_sent} telemetry packets...")
            
            time.sleep(0.5)  # Kirim setiap 500ms
            
        except Exception as e:
            print(f"[TX] Error: {e}")
            time.sleep(1)

# ==========================================
# MAIN
# ==========================================

print("="*60)
print("CRSF Bidirectional Test - RX + TX Simultaneously")
print("="*60)
print(f"Port: {PORT} @ {BAUD} baud")
print()
print("IMPORTANT CHECKS:")
print("1. ✅ RX2 TX pin → USB Adapter RX pin (for receiving RC)")
print("2. ❓ RX2 RX pin → USB Adapter TX pin (for sending telemetry)")
print("   👆 THIS IS CRITICAL! Telemetry won't work without this!")
print()
print("3. RadioMaster Pocket Settings:")
print("   - Model Settings → External RF → Telemetry: ON")
print("   - Set Telem Ratio to 1:128 or 1:64")
print()
print("4. Check telemetry on radio:")
print("   - Press SYS button → Telemetry page")
print("   - Should see Battery voltage, current, etc.")
print("="*60)
print()

# Start threads
rx_thread = threading.Thread(target=receive_thread, daemon=True)
tx_thread = threading.Thread(target=telemetry_thread, daemon=True)

rx_thread.start()
tx_thread.start()

# Monitor
try:
    print("Running... Press Ctrl+C to stop\n")
    while True:
        time.sleep(5)
        if rx_active:
            print(f"📊 Status: RC receiving ✅ | Telemetry sent: {telemetry_sent} packets")
        else:
            print(f"⚠️  Status: No RC data received yet... | Telemetry sent: {telemetry_sent} packets")
            
except KeyboardInterrupt:
    print("\n\nStopped by user")
    ser.close()
