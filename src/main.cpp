#ifdef ARDUINO
// ESP32 / Arduino build

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <cstring>
#include "game_engine.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
GameEngine engine;

void displayFrame(const uint8_t* buffer) {
  uint8_t* displayBuf = display.getBuffer();
  memset(displayBuf, 0, SCREEN_WIDTH * SCREEN_HEIGHT / 8);

  for (int gameY = 0; gameY < Graphics::HEIGHT; gameY++) {
    for (int gameX = 0; gameX < Graphics::WIDTH; gameX++) {
      int gameByte = (gameY * Graphics::WIDTH + gameX) / 8;
      int gameBit = gameX % 8;
      bool pixel = (buffer[gameByte] >> gameBit) & 1;

      if (pixel) {
        int oledX = gameY;
        int oledY = 63 - gameX;
        displayBuf[oledX + (oledY / 8) * 128] |= (1 << (oledY & 7));
      }
    }
  }
  display.display();
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Initializing OLED...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Initializing game...");
  display.display();

  Serial.println("Initializing game engine...");
  engine.initialize();

  Serial.println("Ready!");
}

void loop() {
  static unsigned long lastTime = 0;
  unsigned long now = millis();
  float deltaTime = (now - lastTime) / 1000.0f;
  lastTime = now;

  engine.update(deltaTime);
  engine.render();

  displayFrame(engine.getGraphics().getBuffer());

  Serial.printf("Score: %d, Balls: %d\n", engine.getScore(),
                engine.getBallsRemaining());

  delay(33); // ~30 FPS
}

#else
// Native desktop build for testing

#include "game_engine.h"
#include <cstdio>
#include <chrono>
#include <thread>

void debugRender(const Graphics& g) {
  printf("\033[2J\033[H"); // Clear screen
  for (int y = 0; y < Graphics::HEIGHT; y++) {
    for (int x = 0; x < Graphics::WIDTH; x++) {
      printf(g.getPixel(x, y) ? "█" : " ");
    }
    printf("\n");
  }
}

int main() {
  GameEngine engine;
  engine.initialize();

  auto lastTime = std::chrono::high_resolution_clock::now();

  for (int frame = 0; frame < 1000; frame++) {
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime =
        std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;

    engine.update(deltaTime);
    engine.render();

    debugRender(engine.getGraphics());
    printf("Score: %d, Balls: %d\n", engine.getScore(),
           engine.getBallsRemaining());

    std::this_thread::sleep_for(std::chrono::milliseconds(33));
  }

  return 0;
}

#endif
