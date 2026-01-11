/*
 * FishBot Motor + Encoder Test Program
 * Board: ESP32 Dev Module
 * Testing: Motor running + Encoder reading
 * 
 * GPIO Mapping:
 * Motor 0 (Kanan): GPIO 22 (Forward), 23 (Backward)
 * Motor 1 (Kiri):  GPIO 12 (Forward), 13 (Backward)
 * Encoder 0 (Kanan): GPIO 32 (A), 33 (B)
 * Encoder 1 (Kiri):  GPIO 26 (A), 25 (B)
 */

// ===== MOTOR PIN DEFINITIONS =====
#define MOTOR0_A 22  // Motor Kanan Forward
#define MOTOR0_B 23  // Motor Kanan Backward
#define MOTOR1_A 12  // Motor Kiri Forward
#define MOTOR1_B 13  // Motor Kiri Backward

// ===== ENCODER PIN DEFINITIONS =====
#define ENCODER0_A 32  // Encoder Kanan A
#define ENCODER0_B 33  // Encoder Kanan B
#define ENCODER1_A 26  // Encoder Kiri A
#define ENCODER1_B 25  // Encoder Kiri B

// LED untuk indikator
#define LED_PIN 2

// ===== TIMING CONFIGURATION =====
#define MOTOR_RUN_TIME 5000    // Motor jalan 5 detik
#define MOTOR_STOP_TIME 2000   // Motor stop 2 detik

// ===== GLOBAL VARIABLES =====
volatile long encoder0_count = 0;
volatile long encoder1_count = 0;

volatile int encoder0_direction = 0;
volatile int encoder1_direction = 0;

unsigned long lastPrintTime = 0;
unsigned long motorStateChangeTime = 0;
bool motorRunning = false;
int testCycle = 0;

// ===== ISR (Interrupt Service Routines) =====

void IRAM_ATTR encoder0_ISR() {
  int b = digitalRead(ENCODER0_B);
  
  if (b == HIGH) {
    encoder0_count++;
    encoder0_direction = 1;
  } else {
    encoder0_count--;
    encoder0_direction = -1;
  }
}

void IRAM_ATTR encoder1_ISR() {
  int b = digitalRead(ENCODER1_B);
  
  if (b == HIGH) {
    encoder1_count++;
    encoder1_direction = 1;
  } else {
    encoder1_count--;
    encoder1_direction = -1;
  }
}

// ===== MOTOR CONTROL FUNCTIONS =====

void motorsForward() {
  digitalWrite(MOTOR0_A, HIGH);
  digitalWrite(MOTOR0_B, LOW);
  digitalWrite(MOTOR1_A, HIGH);
  digitalWrite(MOTOR1_B, LOW);
}

void motorsStop() {
  digitalWrite(MOTOR0_A, LOW);
  digitalWrite(MOTOR0_B, LOW);
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=============================================");
  Serial.println("  FishBot Motor + Encoder Test Program");
  Serial.println("=============================================");
  Serial.println("Board: ESP32 Motion Control v1.8.0");
  Serial.println("Testing: Motor Forward + Encoder Reading");
  Serial.println("=============================================\n");

  // Setup LED
  pinMode(LED_PIN, OUTPUT);
  
  // Setup Motor pins
  pinMode(MOTOR0_A, OUTPUT);
  pinMode(MOTOR0_B, OUTPUT);
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  
  // Setup Encoder pins
  pinMode(ENCODER0_A, INPUT_PULLUP);
  pinMode(ENCODER0_B, INPUT_PULLUP);
  pinMode(ENCODER1_A, INPUT_PULLUP);
  pinMode(ENCODER1_B, INPUT_PULLUP);
  
  // Attach interrupts
  attachInterrupt(digitalPinToInterrupt(ENCODER0_A), encoder0_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODER1_A), encoder1_ISR, RISING);
  
  // Stop motors initially
  motorsStop();
  
  Serial.println("Configuration:");
  Serial.println("- Motor 0 (Right): GPIO 22, 23");
  Serial.println("- Motor 1 (Left):  GPIO 12, 13");
  Serial.println("- Encoder 0 (Right): GPIO 32 (A), 33 (B)");
  Serial.println("- Encoder 1 (Left):  GPIO 26 (A), 25 (B)\n");
  
  Serial.println("Test Sequence:");
  Serial.printf("- Motor RUN:  %d seconds (Forward)\n", MOTOR_RUN_TIME/1000);
  Serial.printf("- Motor STOP: %d seconds\n", MOTOR_STOP_TIME/1000);
  Serial.println("- Repeat continuously\n");
  
  Serial.println("=============================================\n");
  
  delay(2000);
  motorStateChangeTime = millis();
  
  Serial.println("Starting test...\n");
  printHeader();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Motor state machine
  if (motorRunning) {
    // Motor sedang jalan
    if (currentTime - motorStateChangeTime >= MOTOR_RUN_TIME) {
      // Waktunya stop
      motorsStop();
      motorRunning = false;
      motorStateChangeTime = currentTime;
      digitalWrite(LED_PIN, LOW);
      
      Serial.println("├─────────┼──────────┼──────────┼──────────┼──────────┼─────────────┤");
      Serial.println("│  MOTOR STOPPED - Analyzing results...                            │");
      Serial.println("└───────────────────────────────────────────────────────────────────┘\n");
      
      printSummary();
    }
  } else {
    // Motor sedang stop
    if (currentTime - motorStateChangeTime >= MOTOR_STOP_TIME) {
      // Waktunya jalan lagi
      testCycle++;
      encoder0_count = 0;  // Reset counter
      encoder1_count = 0;
      
      Serial.println("\n┌───────────────────────────────────────────────────────────────────┐");
      Serial.printf("│  TEST CYCLE #%d - MOTOR RUNNING FORWARD                          │\n", testCycle);
      Serial.println("└───────────────────────────────────────────────────────────────────┘\n");
      printHeader();
      
      motorsForward();
      motorRunning = true;
      motorStateChangeTime = currentTime;
      digitalWrite(LED_PIN, HIGH);
    }
  }
  
  // Print encoder data setiap 200ms saat motor jalan
  if (motorRunning && (currentTime - lastPrintTime >= 200)) {
    lastPrintTime = currentTime;
    printEncoderData();
  }
  
  delay(10);
}

void printHeader() {
  Serial.println("┌─────────┬──────────┬──────────┬──────────┬──────────┬─────────────┐");
  Serial.println("│  Time   │ Encoder0 │  Speed0  │ Encoder1 │  Speed1  │   Status    │");
  Serial.println("│  (sec)  │  (Right) │ (pls/s)  │  (Left)  │ (pls/s)  │             │");
  Serial.println("├─────────┼──────────┼──────────┼──────────┼──────────┼─────────────┤");
}

void printEncoderData() {
  static long last_enc0 = 0;
  static long last_enc1 = 0;
  static unsigned long last_time = 0;
  
  unsigned long current_time = millis();
  float dt = (current_time - last_time) / 1000.0;  // delta time in seconds
  
  // Calculate speed (pulses per second)
  int speed0 = 0;
  int speed1 = 0;
  
  if (dt > 0) {
    speed0 = (encoder0_count - last_enc0) / dt;
    speed1 = (encoder1_count - last_enc1) / dt;
  }
  
  last_enc0 = encoder0_count;
  last_enc1 = encoder1_count;
  last_time = current_time;
  
  // Status string
  const char* status = (speed0 > 0 && speed1 > 0) ? "  ✓ GOOD   " : 
                       (speed0 == 0 && speed1 == 0) ? " ✗ NO READ " :
                       (speed0 == 0) ? " ✗ ENC0 ERR" :
                       (speed1 == 0) ? " ✗ ENC1 ERR" : "  ⚠ CHECK  ";
  
  char buffer[100];
  sprintf(buffer, "│ %6.1f  │  %6ld  │  %6d  │  %6ld  │  %6d  │ %s │",
          current_time / 1000.0,
          encoder0_count,
          speed0,
          encoder1_count,
          speed1,
          status);
  
  Serial.println(buffer);
}

void printSummary() {
  Serial.println("┌───────────────────────────────────────────────────────────────────┐");
  Serial.println("│                        TEST CYCLE SUMMARY                         │");
  Serial.println("├───────────────────────────────────────────────────────────────────┤");
  
  char buffer[100];
  
  // Total pulses
  sprintf(buffer, "│ Total Encoder 0 (Right): %8ld pulses                           │", encoder0_count);
  Serial.println(buffer);
  sprintf(buffer, "│ Total Encoder 1 (Left):  %8ld pulses                           │", encoder1_count);
  Serial.println(buffer);
  
  // Calculate distance (assuming 1782 pulses per wheel revolution, 65mm wheel diameter)
  float wheel_circumference = 3.14159 * 65.0;  // mm
  float pulses_per_rev = 1782.0;
  
  float distance0 = (encoder0_count / pulses_per_rev) * wheel_circumference / 10.0;  // cm
  float distance1 = (encoder1_count / pulses_per_rev) * wheel_circumference / 10.0;  // cm
  
  sprintf(buffer, "│                                                                   │");
  Serial.println(buffer);
  sprintf(buffer, "│ Calculated Distance:                                              │");
  Serial.println(buffer);
  sprintf(buffer, "│   Motor 0 (Right): %6.2f cm                                      │", distance0);
  Serial.println(buffer);
  sprintf(buffer, "│   Motor 1 (Left):  %6.2f cm                                      │", distance1);
  Serial.println(buffer);
  
  // Average speed
  float avg_speed0 = encoder0_count / (MOTOR_RUN_TIME / 1000.0);
  float avg_speed1 = encoder1_count / (MOTOR_RUN_TIME / 1000.0);
  
  sprintf(buffer, "│                                                                   │");
  Serial.println(buffer);
  sprintf(buffer, "│ Average Speed:                                                    │");
  Serial.println(buffer);
  sprintf(buffer, "│   Motor 0 (Right): %6.1f pulses/sec                              │", avg_speed0);
  Serial.println(buffer);
  sprintf(buffer, "│   Motor 1 (Left):  %6.1f pulses/sec                              │", avg_speed1);
  Serial.println(buffer);
  
  // Analysis
  Serial.println("│                                                                   │");
  Serial.println("│ Analysis:                                                         │");
  
  if (encoder0_count > 0 && encoder1_count > 0) {
    Serial.println("│   ✓ Both encoders working correctly!                             │");
    
    // Check balance
    float diff_percent = abs(encoder0_count - encoder1_count) * 100.0 / max(encoder0_count, encoder1_count);
    
    if (diff_percent < 5) {
      Serial.println("│   ✓ Motors are well balanced (< 5% difference)                   │");
    } else if (diff_percent < 10) {
      Serial.println("│   ⚠ Motors slightly unbalanced (5-10% difference)                │");
    } else {
      Serial.println("│   ✗ Motors significantly unbalanced (> 10% difference)           │");
      Serial.println("│     → Check motor connections or mechanical issues               │");
    }
  } else if (encoder0_count == 0 && encoder1_count == 0) {
    Serial.println("│   ✗ NO ENCODER READING!                                          │");
    Serial.println("│     → Check encoder connections                                   │");
    Serial.println("│     → Check encoder power supply                                  │");
    Serial.println("│     → Verify motors are actually running                          │");
  } else if (encoder0_count == 0) {
    Serial.println("│   ✗ Encoder 0 (Right) NOT WORKING!                               │");
    Serial.println("│     → Check GPIO 32, 33 connections                               │");
  } else if (encoder1_count == 0) {
    Serial.println("│   ✗ Encoder 1 (Left) NOT WORKING!                                │");
    Serial.println("│     → Check GPIO 26, 25 connections                               │");
  }
  
  Serial.println("└───────────────────────────────────────────────────────────────────┘");
}

/*
 * ===== CARA MENGGUNAKAN =====
 * 
 * 1. Upload program ke ESP32 FishBot
 * 2. Buka Serial Monitor (115200 baud)
 * 3. Letakkan robot di tempat yang aman (tidak menabrak)
 * 4. Motor akan otomatis:
 *    - Jalan forward 5 detik
 *    - Stop 2 detik
 *    - Show summary & analysis
 *    - Repeat
 * 5. Monitor output encoder dan speed
 * 
 * ===== INTERPRETASI HASIL =====
 * 
 * ✓ ENCODER BAGUS jika:
 * - Count bertambah saat motor jalan
 * - Speed stabil (tidak 0)
 * - Kedua encoder nilai mirip (balance)
 * - Status menunjukkan "✓ GOOD"
 * 
 * ✗ ENCODER BERMASALAH jika:
 * - Count tidak berubah (tetap 0)
 * - Speed = 0 padahal motor jalan
 * - Hanya satu encoder yang bekerja
 * - Status "✗ NO READ" atau error
 * 
 * ===== EXPECTED RESULTS =====
 * 
 * Untuk motor DC kecil dengan reduction 40.5:1:
 * - Typical speed: 200-800 pulses/sec
 * - Distance per 5 sec: 5-20 cm (tergantung voltage)
 * - Balance difference: < 10%
 * 
 * ===== TROUBLESHOOTING =====
 * 
 * Motor jalan tapi encoder 0?
 * → Check koneksi encoder ke board
 * → Verify encoder power (3.3V/5V)
 * → Test dengan multimeter pada pin encoder
 * 
 * Count sangat rendah?
 * → Normal jika voltage rendah (5V)
 * → Motor lambat = encoder count rendah
 * → Try dengan voltage lebih tinggi (7.4V)
 * 
 * Kedua encoder beda jauh?
 * → Check mechanical: ada yang macet?
 * → Check motor: keduanya dapat power sama?
 * → Calibrate motor power jika perlu
 * 
 * ===== MODIFICATIONS =====
 * 
 * Ubah durasi test:
 * #define MOTOR_RUN_TIME 3000   // 3 detik
 * 
 * Ubah update rate:
 * if (currentTime - lastPrintTime >= 100)  // 100ms update
 * 
 * Test backward:
 * Ganti motorsForward() dengan:
 * digitalWrite(MOTOR0_A, LOW);
 * digitalWrite(MOTOR0_B, HIGH);
 * digitalWrite(MOTOR1_A, LOW);
 * digitalWrite(MOTOR1_B, HIGH);
 */
