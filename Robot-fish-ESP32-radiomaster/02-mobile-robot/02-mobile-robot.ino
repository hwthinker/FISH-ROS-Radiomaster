#include <Arduino.h>
#include <ESP32Servo.h>

// ================= CRSF Setup =================
#define CRSF_BAUD 420000
#define CRSF_RX_PIN 21   // Radio TX -> ESP32 RX
#define CRSF_TX_PIN 26   // Radio RX -> ESP32 TX (opsional, diubah dari 22)
HardwareSerial CRSF(1);

// ================= Servo Setup =================
Servo yawServo;
const int SERVO_PIN = 27;  // Servo YAW di GPIO27

// ================= Motor Pins =================
#define MOTOR1_A 22  // Motor Kanan CW/Forward
#define MOTOR1_B 23  // Motor Kanan CCW/Backward
#define MOTOR2_A 12  // Motor Kiri CW/Forward
#define MOTOR2_B 13  // Motor Kiri CCW/Backward

// PWM properties
const int PWM_FREQ = 1000;

// ================= CRSF Variables =================
uint16_t channels[16];

// ================= Decode CRSF =================
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

// Konversi PWM (1000-2000) ke sudut servo (0-180)
int pwmToServoAngle(int pwm) {
  return map(pwm, 1000, 2000, 0, 180);
}

// Konversi PWM (1000-2000) ke kecepatan motor (0-255)
// Dead zone di tengah untuk stop
void controlMotors(int pitchPWM) {
  const int DEAD_ZONE = 50; // Dead zone +/- 50 dari center (1500)
  int center = 1500;
  
  if (pitchPWM > center + DEAD_ZONE) {
    // Forward - pitch stick ke depan
    int speed = map(pitchPWM, center + DEAD_ZONE, 2000, 0, 255);
    speed = constrain(speed, 0, 255);
    
    // Motor kanan forward
    ledcWrite(MOTOR1_A, speed);
    ledcWrite(MOTOR1_B, 0);
    
    // Motor kiri forward
    ledcWrite(MOTOR2_A, speed);
    ledcWrite(MOTOR2_B, 0);
    
  } else if (pitchPWM < center - DEAD_ZONE) {
    // Backward - pitch stick ke belakang
    int speed = map(pitchPWM, 1000, center - DEAD_ZONE, 255, 0);
    speed = constrain(speed, 0, 255);
    
    // Motor kanan backward
    ledcWrite(MOTOR1_A, 0);
    ledcWrite(MOTOR1_B, speed);
    
    // Motor kiri backward
    ledcWrite(MOTOR2_A, 0);
    ledcWrite(MOTOR2_B, speed);
    
  } else {
    // Stop - stick di tengah (dead zone)
    ledcWrite(MOTOR1_A, 0);
    ledcWrite(MOTOR1_B, 0);
    ledcWrite(MOTOR2_A, 0);
    ledcWrite(MOTOR2_B, 0);
  }
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
  
  // Inisialisasi CRSF
  CRSF.begin(CRSF_BAUD, SERIAL_8N1, CRSF_RX_PIN, CRSF_TX_PIN);
  
  // Inisialisasi Servo
  yawServo.attach(SERVO_PIN);
  yawServo.write(90); // Posisi tengah saat startup
  
  // Inisialisasi Motor PWM (ESP32 Arduino Core 3.x API)
  ledcAttach(MOTOR1_A, PWM_FREQ, 8); // 8-bit resolution
  ledcAttach(MOTOR1_B, PWM_FREQ, 8);
  ledcAttach(MOTOR2_A, PWM_FREQ, 8);
  ledcAttach(MOTOR2_B, PWM_FREQ, 8);
  
  // Stop semua motor
  ledcWrite(MOTOR1_A, 0);
  ledcWrite(MOTOR1_B, 0);
  ledcWrite(MOTOR2_A, 0);
  ledcWrite(MOTOR2_B, 0);
  
  Serial.println("================================================");
  Serial.println("RadioMaster Pocket + Servo + Motor Ready!");
  Serial.println("YAW channel  -> Servo on GPIO27");
  Serial.println("PITCH channel -> Drive Motors (Forward/Backward)");
  Serial.println("================================================");
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
    
    // ================= KONTROL SERVO DENGAN YAW =================
    int servoAngle = pwmToServoAngle(yaw);
    yawServo.write(servoAngle);
    
    // ================= KONTROL MOTOR DENGAN PITCH =================
    controlMotors(pitch);
    
    // Determine motor direction for display
    String motorDir = "STOP";
    if (pitch > 1550) motorDir = "FWD";
    else if (pitch < 1450) motorDir = "BWD";
    
    // Print data dengan info servo dan motor
    Serial.printf(
      "PITCH:%4d [%s]  YAW:%4d [%3d°]  |  "
      "THR:%4d  ROLL:%4d  |  SA:%s SB:%s SC:%s SD:%s\n",
      pitch, motorDir.c_str(), yaw, servoAngle,
      throttle, roll,
      swA, swB, swC, swD
    );
  }
}