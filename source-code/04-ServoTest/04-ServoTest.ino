#include <ESP32Servo.h>

// Buat objek servo
Servo myServo;

// Pin GPIO untuk servo (bisa diubah sesuai kebutuhan)
const int servoPin = 27;

void setup() {
  // Inisialisasi serial monitor
  Serial.begin(115200);
  
  // Attach servo ke pin yang ditentukan
  // ESP32Servo menggunakan timer dan channel secara otomatis
  myServo.attach(servoPin);
  
  Serial.println("Servo MG996R siap digunakan!");
  Serial.println("Servo akan bergerak otomatis...");
}

void loop() {
  // Gerakkan servo dari 0 ke 180 derajat secara bertahap
  Serial.println("Bergerak ke 0 derajat");
  myServo.write(0);
  delay(1000);
  
  Serial.println("Bergerak ke 90 derajat");
  myServo.write(90);
  delay(1000);
  
  Serial.println("Bergerak ke 180 derajat");
  myServo.write(180);
  delay(1000);
  
  Serial.println("Bergerak ke 90 derajat");
  myServo.write(90);
  delay(1000);
  
  // Gerakan halus dari 0 ke 180 derajat
  Serial.println("Gerakan halus 0-180");
  for(int pos = 0; pos <= 180; pos++) {
    myServo.write(pos);
    delay(15);
  }
  
  // Gerakan halus dari 180 ke 0 derajat
  Serial.println("Gerakan halus 180-0");
  for(int pos = 180; pos >= 0; pos--) {
    myServo.write(pos);
    delay(15);
  }
  
  delay(1000);
}
