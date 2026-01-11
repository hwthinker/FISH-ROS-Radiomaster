import serial
import time

PORT = "COM10"
BAUD = 420000
ser = serial.Serial(PORT, BAUD, timeout=0.1)

def crc8(data):
    crc = 0
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = (crc << 1) ^ 0xD5 if crc & 0x80 else crc << 1
            crc &= 0xFF
    return crc

def send_battery(voltage_mv, current_ma, capacity_mah, percent):
    payload = bytearray()
    payload.append(0x02)                       # Battery sensor
    payload += voltage_mv.to_bytes(2, "big")   # mV
    payload += current_ma.to_bytes(2, "big")   # mA
    payload += capacity_mah.to_bytes(3, "big") # mAh (24 bit)
    payload.append(percent & 0xFF)

    frame = bytearray()
    frame.append(0xEA)                         # ✅ FC → TX
    frame.append(len(payload) + 1)
    frame += payload
    frame.append(crc8(payload))

    ser.write(frame)

def send_text(msg):
    msg = msg[:15].encode("ascii")
    payload = bytearray([0x21]) + msg

    frame = bytearray()
    frame.append(0xEA)                         # ✅ FC → TX
    frame.append(len(payload) + 1)
    frame += payload
    frame.append(crc8(payload))

    ser.write(frame)

print("Sending VALID CRSF telemetry...")

while True:
    send_battery(
        voltage_mv=12600,
        current_ma=5200,
        capacity_mah=850,
        percent=75
    )
    send_text("MODE:MANUAL")
    time.sleep(0.5)
