#include <Arduino.h>

#define CRSF_BAUD 420000
#define CRSF_RX_PIN 21   // Radio TX -> ESP32 RX
#define CRSF_TX_PIN 27   // Radio RX -> ESP32 TX (opsional)

HardwareSerial CRSF(1);

// ================= Decode CRSF =================
uint16_t channels[16];

void decodeChannels(uint8_t *payload, int len) {
  uint32_t bitBuffer = 0;
  uint8_t bitCount = 0;
  int chIndex = 0;

  for (int i = 0; i < len && chIndex < 16; i++) {
    bitBuffer |= (uint32_t)payload[i] << bitCount;
    bitCount += 8;

    while (bitCount >= 11 && chIndex < 16) {
      channels[chIndex++] = bitBuffer & 0x7FF;
      bitBuffer >>= 11;
      bitCount -= 11;
    }
  }
}

int crsfToPWM(uint16_t c) {
  return (int)((c - 172) * 1000.0 / (1811 - 172) + 1000);
}

const char* switchState(int pwm) {
  if (pwm < 1300) return "OFF";
  else if (pwm < 1700) return "MID";
  else return "ON";
}

// ================= Setup =================
void setup() {
  Serial.begin(115200);
  delay(1000);

  CRSF.begin(CRSF_BAUD, SERIAL_8N1, CRSF_RX_PIN, CRSF_TX_PIN);

  Serial.println("Reading RadioMaster Pocket input...");
}

// ================= Loop =================
void loop() {
  static uint8_t frame[64];

  if (!CRSF.available()) return;

  // CRSF frame start byte
  if (CRSF.read() != 0xC8) return;

  while (!CRSF.available());
  uint8_t length = CRSF.read();

  for (int i = 0; i < length; i++) {
    while (!CRSF.available());
    frame[i] = CRSF.read();
  }

  uint8_t frameType = frame[0];

  // RC Channels Packed
  if (frameType == 0x16) {
    uint8_t *payload = &frame[1];
    int payloadLen = length - 2; // minus type & CRC

    decodeChannels(payload, payloadLen);

    int pwm[16];
    for (int i = 0; i < 16; i++) {
      pwm[i] = crsfToPWM(channels[i]);
    }

    // Axis
    int roll     = pwm[0];
    int pitch    = pwm[1];
    int throttle = pwm[2];
    int yaw      = pwm[3];

    // Switch SA–SD
    const char* swA = switchState(pwm[4]);
    const char* swB = switchState(pwm[5]);
    const char* swC = switchState(pwm[6]);
    const char* swD = switchState(pwm[7]);

    // Tambahan
    const char* swE = switchState(pwm[8]); // CH9
    int knobS1 = pwm[9];                   // CH10

    Serial.printf(
      "THR:%4d  ROLL:%4d  PITCH:%4d  YAW:%4d  |  "
      "SA:%s SB:%s SC:%s SD:%s  SE:%s  S1:%4d\n",
      throttle, roll, pitch, yaw,
      swA, swB, swC, swD, swE, knobS1
    );
  }
}