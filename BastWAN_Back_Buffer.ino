#include <Adafruit_GFX.h>
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include <BBQ10Keyboard.h>

float r0 = .75;
uint32_t offsetX = 75, offsetY = 100, barWidth = 160, barHeight = 20;
#define ILI9341_CS 9
#define ILI9341_DC 10
#define SD_CS 5
Adafruit_ILI9341 tft(&SPI1, ILI9341_DC, ILI9341_CS, -1);
BBQ10Keyboard keyboard;

void setup() {
  // ---- HOUSEKEEPING ----
  SerialUSB.begin(115200);
  SerialUSB.flush();
  delay(3000);
  SerialUSB.println("\n\nBack Buffer");
  // ---- TFT STUFF ----
  tft.begin();
  SerialUSB.println(" . TFT begin");
  tft.fillScreen(ILI9341_WHITE);
  tft.setRotation(1);
  // ---- KBD STUFF ----
  keyboard.begin();
  SerialUSB.println(" . Keyboard begin");
  keyboard.setBacklight(0.00f); delay(200);
  keyboard.setBacklight(0.50f);
  drawSlider();
  SerialUSB.println("Less gow");
}

void loop() {
  const int keyCount = keyboard.keyCount();
  if (keyCount > 0) {
    BBQ10Keyboard::KeyEvent key = keyboard.keyEvent();
    if (key.state == BBQ10Keyboard::StateLongPress) {
      Serial.println("StateLongPress");
      float modif = 0.1; // RIGHT
      if (key.key == 3) modif = -0.1; // LEFT
      while (key.state != BBQ10Keyboard::StateRelease) {
        r0 += modif;
        if (r0 > 1.0) r0 = 1.0;
        if (r0 < 0.0) r0 = 0.0;
        drawSlider();
        delay(10);
        key = keyboard.keyEvent();
      }
      Serial.println("StateLongPress end");
    }
    if (key.state == BBQ10Keyboard::StatePress) {
      if (key.key == 4) {
        // RIGHT
        r0 += .01;
        if (r0 > 1.0) r0 = 1.0;
        drawSlider();
      } else if (key.key == 3) {
        // LEFT
        r0 -= .01;
        if (r0 < 0.0) r0 = 0.0;
        drawSlider();
      }
      key = keyboard.keyEvent();
    }
    if (key.state == BBQ10Keyboard::StateRelease) {
      if (key.key == 4) {
        // RIGHT
        drawSlider();
      } else if (key.key == 3) {
        // LEFT
        drawSlider();
      }
    }
  }
}


void drawSlider() {
  uint8_t i, j;
  uint16_t lm = barWidth * r0;
  uint16_t rm = barWidth - lm;
  Serial.println("lm = " + String(lm));
  Serial.println("r0 = " + String(r0));
  tft.startWrite();
  tft.writeFastHLine(offsetX, offsetY, barWidth + 2, ILI9341_RED);
  tft.writeFastHLine(offsetX, offsetY + barHeight + 1, barWidth + 2, ILI9341_RED);
  tft.writeFastVLine(offsetX, offsetY, barHeight + 2, ILI9341_RED);
  tft.writeFastVLine(offsetX + barWidth + 2, offsetY, barHeight + 2, ILI9341_RED);
  for (j = 0; j < barHeight; j++) {
    if (lm == barWidth) {
      tft.writeFastHLine(offsetX + 1, offsetY + j + 1, barWidth, ILI9341_BLUE);
    } else if (lm == 0) {
      tft.writeFastHLine(offsetX + 1, offsetY + j + 1, barWidth, ILI9341_WHITE);
    } else {
      tft.writeFastHLine(offsetX + 1, offsetY + j + 1, lm, ILI9341_BLUE);
      tft.writeFastHLine(offsetX + lm + 2, offsetY + j + 1, rm, ILI9341_WHITE);
    }
  }
  tft.endWrite();
}
