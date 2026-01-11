import serial

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

print("Reading RadioMaster Pocket input...")

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

