// ESP32 + SSD1306 0.96" I2C (0x3C)
// SDA = 18, SCL = 19

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CONFIG_DEFAULT_OLED_SDA_GPIO 18
#define CONFIG_DEFAULT_OLED_SCL_GPIO 19

// Ubah ke 128x32 jika modul kamu 128x32
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// -1 karena umumnya modul OLED tidak punya pin RESET terpisah
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(100);

  // Inisialisasi I2C di pin yang diinginkan
  Wire.begin(CONFIG_DEFAULT_OLED_SDA_GPIO, CONFIG_DEFAULT_OLED_SCL_GPIO);

  // Coba init OLED di alamat 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Gagal init SSD1306! Cek wiring/alamat I2C.");
    while (true) { delay(10); }
  }

  // Efek splash singkat
  display.clearDisplay();
  display.setTextSize(1);              // Skala font
  display.setTextColor(SSD1306_WHITE); // Putih-monokrom
  display.setCursor(0, 0);
  display.println("ESP32 + SSD1306");
  display.println("I2C 0x3C, SDA=18");
  display.println("SCL=19");
  display.display();
  delay(1500);

  // Contoh grafis: garis, kotak, lingkaran
  display.clearDisplay();
  // Kotak border
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  // Garis diagonal
  display.drawLine(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, SSD1306_WHITE);
  display.drawLine(0, SCREEN_HEIGHT-1, SCREEN_WIDTH-1, 0, SSD1306_WHITE);
  // Lingkaran tengah
  display.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 12, SSD1306_WHITE);
  display.display();
  delay(1200);

  // Teks besar di tengah
  display.clearDisplay();
  centerText("HELLO", 0, 2);        // baris 0, ukuran 2
  centerText("SSD1306", 24, 2);     // y=24
  display.display();
}

void loop() {
  // Demo counter berjalan
  static uint32_t t0 = 0;
  static uint32_t counter = 0;

  if (millis() - t0 > 500) { // update tiap 0.5 detik
    t0 = millis();
    display.fillRect(0, 48, SCREEN_WIDTH, 16, SSD1306_BLACK); // bersihkan area bawah
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(2, 50);
    display.print("Counter: ");
    display.println(counter++);
    display.display();
  }
}

// Helper untuk center text (mono-line)
void centerText(const char* text, int16_t y, uint8_t textSize) {
  int16_t x1, y1;
  uint16_t w, h;
  display.setTextSize(textSize);
  display.setTextColor(SSD1306_WHITE);
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  int16_t x = (SCREEN_WIDTH - w) / 2;
  display.setCursor(x, y);
  display.print(text);
}
