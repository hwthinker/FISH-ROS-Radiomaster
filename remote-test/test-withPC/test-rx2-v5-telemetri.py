import serial
import struct
import time

PORT = "COM10"        # ganti
BAUD = 420000

ser = serial.Serial(PORT, BAUD, timeout=0.1)

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

def switch_state(pwm):
    if pwm < 1300:
        return "OFF"
    elif pwm < 1700:
        return "MID"
    else:
        return "ON"

def crc8_dvb_s2(data):
    """Hitung CRC8 untuk CRSF protocol"""
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
    """
    Kirim Battery Sensor telemetry (0x08)
    voltage: dalam Volt (misal 12.6)
    current: dalam Ampere (misal 5.2)
    capacity: dalam mAh used (misal 850)
    percent: battery percentage 0-100
    """
    frame_type = 0x08  # Battery Sensor
    
    # Convert ke format CRSF
    voltage_data = int(voltage * 10)      # dV (deci-volt)
    current_data = int(current * 10)      # dA (deci-ampere)
    capacity_data = int(capacity)         # mAh (0xFFFFFF = 16777215 mAh max)
    percent_data = int(percent)           # 0-100%
    
    # Pack payload
    payload = struct.pack('>BHBHB',
        frame_type,
        voltage_data & 0xFFFF,
        (current_data >> 8) & 0xFF,
        current_data & 0xFFFF,
        percent_data
    )
    
    # Hitung panjang (payload + CRC)
    length = len(payload) + 1
    
    # Header
    frame = bytearray([0xC8, length])
    frame.extend(payload)
    
    # CRC
    crc = crc8_dvb_s2(payload)
    frame.append(crc)
    
    ser.write(frame)

def send_gps_telemetry(lat, lon, altitude, speed, sats):
    """
    Kirim GPS telemetry (0x02)
    lat: latitude dalam degree (misal -7.250445)
    lon: longitude dalam degree (misal 112.768845)
    altitude: dalam meter
    speed: dalam km/h
    sats: jumlah satelit
    """
    frame_type = 0x02  # GPS
    
    # Convert ke format CRSF
    lat_data = int(lat * 1e7)       # degree * 10^7
    lon_data = int(lon * 1e7)       # degree * 10^7
    alt_data = int(altitude + 1000) # meter + 1000 offset
    speed_data = int(speed * 10)    # km/h * 10
    
    # Pack payload - gunakan format yang benar
    payload = struct.pack('>BiiHHB',
        frame_type,
        lat_data,
        lon_data,
        alt_data & 0xFFFF,
        speed_data & 0xFFFF,
        sats & 0xFF
    )
    
    length = len(payload) + 1
    frame = bytearray([0xC8, length])
    frame.extend(payload)
    crc = crc8_dvb_s2(payload)
    frame.append(crc)
    
    ser.write(frame)

def send_attitude_telemetry(pitch, roll, yaw):
    """
    Kirim Attitude telemetry (0x1E)
    pitch, roll, yaw: dalam radian
    """
    frame_type = 0x1E  # Attitude
    
    # Convert ke format CRSF (radians * 10000)
    pitch_data = int(pitch * 10000)
    roll_data = int(roll * 10000)
    yaw_data = int(yaw * 10000)
    
    # Clamp ke range signed short (-32768 to 32767)
    pitch_data = max(-32768, min(32767, pitch_data))
    roll_data = max(-32768, min(32767, roll_data))
    yaw_data = max(-32768, min(32767, yaw_data))
    
    payload = struct.pack('>Bhhh',
        frame_type,
        pitch_data,
        roll_data,
        yaw_data
    )
    
    length = len(payload) + 1
    frame = bytearray([0xC8, length])
    frame.extend(payload)
    crc = crc8_dvb_s2(payload)
    frame.append(crc)
    
    ser.write(frame)

def send_flight_mode(mode_string):
    """
    Kirim Flight Mode text (0x21)
    mode_string: max 16 karakter (misal "STABILIZE", "ACRO")
    """
    frame_type = 0x21  # Flight Mode
    
    # Pastikan max 16 chars
    mode_bytes = mode_string[:16].encode('ascii')
    
    payload = bytearray([frame_type])
    payload.extend(mode_bytes)
    # Pad dengan null jika < 16 chars
    payload.extend(b'\x00' * (16 - len(mode_bytes)))
    
    length = len(payload) + 1
    frame = bytearray([0xC8, length])
    frame.extend(payload)
    crc = crc8_dvb_s2(payload)
    frame.append(crc)
    
    ser.write(frame)

print("Reading RadioMaster Pocket input & sending telemetry...")

# Counter untuk throttle telemetry
telem_counter = 0
last_telem_time = time.time()

while True:
    if ser.read(1) != b'\xC8':
        continue

    length = ser.read(1)[0]
    frame = ser.read(length)
    frame_type = frame[0]

    if frame_type == 0x16:  # RC Channels Packed
        payload = frame[1:-1]
        ch_raw = decode_channels(payload)
        pwm = [crsf_to_pwm(c) for c in ch_raw]

        # Axis
        roll     = pwm[0]
        pitch    = pwm[1]
        throttle = pwm[2]
        yaw      = pwm[3]

        # Switch SA–SD
        swA = switch_state(pwm[4])  # CH5
        swB = switch_state(pwm[5])  # CH6
        swC = switch_state(pwm[6])  # CH7
        swD = switch_state(pwm[7])  # CH8

        # Tambahan
        swE = switch_state(pwm[8])  # CH9 (SE)
        knob_S1 = pwm[9]            # CH10 (S1 analog)

        print(
            f"THR:{throttle:4}  "
            f"ROLL:{roll:4}  "
            f"PITCH:{pitch:4}  "
            f"YAW:{yaw:4}  |  "
            f"SA:{swA} SB:{swB} SC:{swC} SD:{swD}  "
            f"SE:{swE}  "
            f"S1:{knob_S1:4}"
        )

        # Kirim telemetry setiap 100ms (~10Hz)
        current_time = time.time()
        if current_time - last_telem_time >= 0.1:
            telem_counter += 1
            
            # Rotasi berbagai jenis telemetry
            if telem_counter % 4 == 0:
                # Battery telemetry
                send_battery_telemetry(
                    voltage=12.6,    # 12.6V
                    current=5.2,     # 5.2A
                    capacity=850,    # 850mAh used
                    percent=75       # 75%
                )
            elif telem_counter % 4 == 1:
                # GPS telemetry
                send_gps_telemetry(
                    lat=-7.250445,        # Surabaya area
                    lon=112.768845,
                    altitude=15,          # 15m
                    speed=0,              # 0 km/h
                    sats=12               # 12 satelit
                )
            elif telem_counter % 4 == 2:
                # Attitude telemetry
                send_attitude_telemetry(
                    pitch=0.1,   # ~5.7 derajat
                    roll=-0.05,  # ~-2.9 derajat
                    yaw=1.57     # ~90 derajat
                )
            else:
                # Flight mode
                send_flight_mode("STABILIZE")
            
            last_telem_time = current_time
