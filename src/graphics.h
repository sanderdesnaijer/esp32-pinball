#pragma once

#include <cstdint>
#include <cstring>

// 128×64 monochrome display, logical coords: x ∈ [0, 63], y ∈ [0, 127]
class Graphics {
public:
  static constexpr int WIDTH = 64;
  static constexpr int HEIGHT = 128;
  static constexpr int BUFFER_SIZE = (WIDTH * HEIGHT) / 8;

  Graphics();

  void clear();
  void setPixel(int x, int y, bool on);
  bool getPixel(int x, int y) const;
  void drawLine(int x0, int y0, int x1, int y1, bool on);
  void drawRect(int x, int y, int w, int h, bool filled, bool on);
  void drawCircle(int cx, int cy, int r, bool filled, bool on);
  void drawChar(int x, int y, char c, bool on = true);
  void drawString(int x, int y, const char* str, bool on = true);
  void drawStringCentered(int y, const char* str, bool on = true);

  const uint8_t* getBuffer() const { return buffer; }

private:
  uint8_t buffer[BUFFER_SIZE];

  void bresenham(int x0, int y0, int x1, int y1, bool on);
  bool inBounds(int x, int y) const;
};
