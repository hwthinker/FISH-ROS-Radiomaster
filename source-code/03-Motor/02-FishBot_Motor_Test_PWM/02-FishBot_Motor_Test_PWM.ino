/*
 * FishBot Motor Test Program - PWM Version (ESP32 Core 3.x)
 * Board: ESP32 Dev Module
 * Testing: Motor ON/OFF dengan PWM - interval 3 detik
 * 
 * GPIO Mapping dari FishBot Motion Control v1.8.0:
 * Motor 0 (Kanan/Right):
 *   - GPIO 22 (MCPWM0A) -> DRV8833 AIN1 (Forward)
 *   - GPIO 23 (MCPWM0B) -> DRV8833 AIN2 (Backward)
 * 
 * Motor 1 (Kiri/Left):
 *   - GPIO 12 (MCPWM1A) -> DRV8833 BIN1 (Forward)
 *   - GPIO 13 (MCPWM1B) -> DRV8833 BIN2 (Backward)
 */

// ===== PIN DEFINITIONS =====
// Motor 0 (Kanan/Right)
#define MOTOR0_A 22  // Forward
#define MOTOR0_B 23  // Backward

// Motor 1 (Kiri/Left)
#define MOTOR1_A 12  // Forward
#define MOTOR1_B 13  // Backward

// LED Internal untuk indikator
#define LED_PIN 2

// ===== TIMING CONFIGURATION =====
#define MOTOR_ON_TIME 3000   // Motor ON selama 3 detik (ms)
#define MOTOR_OFF_TIME 3000  // Motor OFF selama 3 detik (ms)

// ===== PWM CONFIGURATION =====
#define PWM_FREQ 500        // 5 KHz PWM frequency
#define PWM_RESOLUTION 8     // 8-bit resolution (0-255)
#define MOTOR_SPEED 155      // Kecepatan motor (0-255), 150 = ~60% speed

// ===== GLOBAL VARIABLES =====
unsigned long previousMillis = 0;
bool motorState = false;  // false = OFF, true = ON
int testCycle = 0;

// ===== FUNCTION PROTOTYPES =====
void setupMotorPins();
void motorsForward(int speed);
void motorsBackward(int speed);
void motorsStop();
void blinkLED(int times);

void setup() {
  // Inisialisasi Serial untuk debugging
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n=================================");
  Serial.println("  FishBot Motor Test Program");
  Serial.println("  PWM Version (ESP32 Core 3.x)");
  Serial.println("=================================");
  Serial.println("Board: ESP32 Motion Control v1.8.0");
  Serial.println("Test: Motor ON/OFF - 3 detik interval");
  Serial.println("=================================\n");

  // Setup LED indikator
  pinMode(LED_PIN, OUTPUT);
  
  // Setup motor pins dengan PWM
  setupMotorPins();
  
  // Blink LED 3x sebagai tanda program mulai
  blinkLED(3);
  
  Serial.println("Starting motor test in 2 seconds...");
  delay(2000);
  
  Serial.println("\n--- TEST SEQUENCE START ---\n");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Cek apakah sudah waktunya untuk toggle motor state
  unsigned long interval = motorState ? MOTOR_ON_TIME : MOTOR_OFF_TIME;
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    motorState = !motorState;  // Toggle state
    
    if (motorState) {
      // MOTOR ON - Forward direction
      testCycle++;
      Serial.println("====================================");
      Serial.printf("TEST CYCLE #%d\n", testCycle);
      Serial.println("====================================");
      Serial.println("[MOTOR ON] - Forward Direction");
      Serial.printf("Speed: %d/255 (~%d%%)\n", MOTOR_SPEED, (MOTOR_SPEED*100)/255);
      Serial.printf("Duration: %d seconds\n", MOTOR_ON_TIME/1000);
      
      motorsForward(MOTOR_SPEED);
      digitalWrite(LED_PIN, HIGH);  // LED ON saat motor ON
      
    } else {
      // MOTOR OFF
      Serial.println("\n[MOTOR OFF] - Stopping motors");
      Serial.printf("Duration: %d seconds\n", MOTOR_OFF_TIME/1000);
      Serial.println("------------------------------------\n");
      
      motorsStop();
      digitalWrite(LED_PIN, LOW);   // LED OFF saat motor OFF
    }
  }
  
  // Delay kecil untuk stabilitas
  delay(10);
}

// ===== FUNCTION IMPLEMENTATIONS =====

void setupMotorPins() {
  Serial.println("Configuring motor pins with PWM...");
  
  // Configure PWM untuk semua motor pins menggunakan API baru ESP32 Core 3.x
  // Format: ledcAttach(pin, freq, resolution)
  
  // Motor 0 - Forward (GPIO 22)
  ledcAttach(MOTOR0_A, PWM_FREQ, PWM_RESOLUTION);
  
  // Motor 0 - Backward (GPIO 23)
  ledcAttach(MOTOR0_B, PWM_FREQ, PWM_RESOLUTION);
  
  // Motor 1 - Forward (GPIO 12)
  ledcAttach(MOTOR1_A, PWM_FREQ, PWM_RESOLUTION);
  
  // Motor 1 - Backward (GPIO 13)
  ledcAttach(MOTOR1_B, PWM_FREQ, PWM_RESOLUTION);
  
  // Set semua motor ke OFF state
  motorsStop();
  
  Serial.println("Motor pins configured successfully!");
  Serial.println("- Motor 0 (Right): GPIO 22, 23");
  Serial.println("- Motor 1 (Left):  GPIO 12, 13");
  Serial.println("- PWM Frequency: 5 KHz");
  Serial.println("- PWM Resolution: 8-bit (0-255)");
  Serial.println("- Using ESP32 Core 3.x API\n");
}

void motorsForward(int speed) {
  // Konstrain speed antara 0-255
  speed = constrain(speed, 0, 255);
  
  // Motor 0 (Kanan) - Forward
  ledcWrite(MOTOR0_A, speed);  // GPIO 22 = ON (PWM)
  ledcWrite(MOTOR0_B, 0);      // GPIO 23 = OFF
  
  // Motor 1 (Kiri) - Forward
  ledcWrite(MOTOR1_A, speed);  // GPIO 12 = ON (PWM)
  ledcWrite(MOTOR1_B, 0);      // GPIO 13 = OFF
  
  Serial.println("-> Motor 0 (Right): FORWARD");
  Serial.println("-> Motor 1 (Left):  FORWARD");
}

void motorsBackward(int speed) {
  // Konstrain speed antara 0-255
  speed = constrain(speed, 0, 255);
  
  // Motor 0 (Kanan) - Backward
  ledcWrite(MOTOR0_A, 0);      // GPIO 22 = OFF
  ledcWrite(MOTOR0_B, speed);  // GPIO 23 = ON (PWM)
  
  // Motor 1 (Kiri) - Backward
  ledcWrite(MOTOR1_A, 0);      // GPIO 12 = OFF
  ledcWrite(MOTOR1_B, speed);  // GPIO 13 = ON (PWM)
  
  Serial.println("-> Motor 0 (Right): BACKWARD");
  Serial.println("-> Motor 1 (Left):  BACKWARD");
}

void motorsStop() {
  // Set semua PWM ke 0 (motor berhenti)
  ledcWrite(MOTOR0_A, 0);  // GPIO 22 = OFF
  ledcWrite(MOTOR0_B, 0);  // GPIO 23 = OFF
  ledcWrite(MOTOR1_A, 0);  // GPIO 12 = OFF
  ledcWrite(MOTOR1_B, 0);  // GPIO 13 = OFF
  
  Serial.println("-> Motor 0 (Right): STOPPED");
  Serial.println("-> Motor 1 (Left):  STOPPED");
}

void blinkLED(int times) {
  for(int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(200);
    digitalWrite(LED_PIN, LOW);
    delay(200);
  }
}

/*
 * ===== PERUBAHAN DARI VERSI SEBELUMNYA =====
 * 
 * ESP32 Arduino Core 3.x mengubah API PWM:
 * 
 * VERSI LAMA (Core 2.x):
 * ledcSetup(channel, freq, resolution);
 * ledcAttachPin(pin, channel);
 * 
 * VERSI BARU (Core 3.x):
 * ledcAttach(pin, freq, resolution);
 * 
 * API baru lebih sederhana - tidak perlu define channel lagi!
 * ESP32 akan auto-assign channel secara internal.
 * 
 * ===== CARA MENGGUNAKAN =====
 * 
 * 1. Upload program ini ke ESP32 FishBot board
 * 2. Buka Serial Monitor (115200 baud)
 * 3. Pastikan motor terhubung ke DRV8833
 * 4. Program akan otomatis test motor dengan pola:
 *    - Motor ON (Forward) selama 3 detik dengan PWM
 *    - Motor OFF selama 3 detik
 *    - Repeat cycle
 * 
 * ===== MODIFIKASI YANG BISA DILAKUKAN =====
 * 
 * 1. Ubah durasi ON/OFF:
 *    - Edit #define MOTOR_ON_TIME dan MOTOR_OFF_TIME
 * 
 * 2. Ubah kecepatan motor:
 *    - Edit #define MOTOR_SPEED (0-255)
 *    - 0 = stop, 128 = 50%, 255 = full speed
 * 
 * 3. Ubah PWM frequency:
 *    - Edit #define PWM_FREQ (1000-40000 Hz recommended)
 * 
 * 4. Test arah backward:
 *    - Ganti motorsForward() dengan motorsBackward()
 * 
 * ===== TROUBLESHOOTING =====
 * 
 * Motor tidak berputar?
 * - Cek koneksi power ke DRV8833
 * - Cek koneksi motor ke output DRV8833
 * - Coba naikkan MOTOR_SPEED (misal 200 atau 255)
 * 
 * Motor berputar lambat?
 * - Naikkan nilai MOTOR_SPEED
 * - Cek voltage supply (min 6V untuk motor DC)
 * 
 * Motor berputar arah terbalik?
 * - Tukar kabel motor (+/-) atau
 * - Swap PIN A dan B dalam kode
 * 
 * Compile error?
 * - Pastikan menggunakan ESP32 Arduino Core 3.x
 * - Tools -> Board -> Boards Manager -> ESP32 by Espressif (versi 3.x)
 * 
 * ===== SAFETY NOTES =====
 * 
 * - Pastikan robot tidak menabrak saat test
 * - Gunakan power supply yang cukup (min 1A untuk 2 motor)
 * - Jangan short circuit motor pins
 * - Test di area terbuka dan aman
 */
