/*
  FishBot Four-Driver Stepper Blinky (Final)
  ------------------------------------------
  Semua pin stepper (dua ULN2003: U7 & U8) akan ON/OFF bersamaan.
  Pins berdasarkan schematic asli:
    Stepper1: 32, 33, 22, 23
    Stepper2: 25, 26, 12, 13
  Total: 8 pin GPIO (32, 33, 22, 23, 25, 26, 12, 13)
*/

#define LED_BUILTIN 2

int stepperPins[] = {32, 33, 22, 23, 25, 26, 12, 13};
const int BLINK_MS = 500;

void setAllStepperPins(bool level) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(stepperPins[i], level ? HIGH : LOW);
  }
  digitalWrite(LED_BUILTIN, level ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("\n=== FishBot 4WD Stepper Concurrent Blinky ===");
  Serial.print("Pins: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(stepperPins[i]);
    Serial.print(i == 7 ? '\n' : ',');
    pinMode(stepperPins[i], OUTPUT);
    digitalWrite(stepperPins[i], LOW);
  }
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("All stepper pins initialized.");
}

void loop() {
  Serial.println("ALL HIGH");
  setAllStepperPins(true);
  delay(BLINK_MS);

  Serial.println("ALL LOW");
  setAllStepperPins(false);
  delay(BLINK_MS);
}
