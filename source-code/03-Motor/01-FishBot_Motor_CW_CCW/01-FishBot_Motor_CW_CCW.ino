/*
 * FishBot Motor CW/CCW Test
 * Board: ESP32 Dev Module
 * Testing: M1(CW)-M1(STOP)-M1(CCW)-M1(Stop)-M2(CW)-M2(STOP)-M2(CCW)-M2(Stop)
 * 
 * GPIO Mapping:
 * Motor 1 (Kiri):  GPIO 12 (Forward/CW), 13 (Backward/CCW)
 * Motor 2 (Kanan): GPIO 22 (Forward/CW), 23 (Backward/CCW)
 */

// Motor Pins
#define MOTOR1_A 12  // Motor 1 (Kiri) CW/Forward
#define MOTOR1_B 13  // Motor 1 (Kiri) CCW/Backward
#define MOTOR2_A 22  // Motor 2 (Kanan) CW/Forward
#define MOTOR2_B 23  // Motor 2 (Kanan) CCW/Backward

void setup() {
  Serial.begin(115200);
  Serial.println("\nFishBot Motor CW/CCW Sequence Test");
  Serial.println("Durasi setiap step: 3 detik\n");
  
  // Set semua pin sebagai output
  pinMode(MOTOR1_A, OUTPUT);
  pinMode(MOTOR1_B, OUTPUT);
  pinMode(MOTOR2_A, OUTPUT);
  pinMode(MOTOR2_B, OUTPUT);
  
  // Pastikan semua motor OFF dulu
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, LOW);
  
  delay(2000);
  Serial.println("Test dimulai...\n");
}

void loop() {
  // 1. M1 CW (Clockwise)
  Serial.println("1. M1 CW (Clockwise)");
  digitalWrite(MOTOR1_A, HIGH);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, LOW);
  delay(3000);
  
  // 2. M1 STOP
  Serial.println("2. M1 STOP");
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
  delay(3000);
  
  // 3. M1 CCW (Counter-Clockwise)
  Serial.println("3. M1 CCW (Counter-Clockwise)");
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, HIGH);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, LOW);
  delay(3000);
  
  // 4. M1 STOP
  Serial.println("4. M1 STOP");
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
  delay(3000);
  
  // 5. M2 CW (Clockwise)
  Serial.println("5. M2 CW (Clockwise)");
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, HIGH);
  digitalWrite(MOTOR2_B, LOW);
  delay(3000);
  
  // 6. M2 STOP
  Serial.println("6. M2 STOP");
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, LOW);
  delay(3000);
  
  // 7. M2 CCW (Counter-Clockwise)
  Serial.println("7. M2 CCW (Counter-Clockwise)");
  digitalWrite(MOTOR1_A, LOW);
  digitalWrite(MOTOR1_B, LOW);
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, HIGH);
  delay(3000);
  
  // 8. M2 STOP
  Serial.println("8. M2 STOP\n");
  digitalWrite(MOTOR2_A, LOW);
  digitalWrite(MOTOR2_B, LOW);
  delay(3000);
  
  Serial.println("--- Cycle Complete, Repeat ---\n");
}
