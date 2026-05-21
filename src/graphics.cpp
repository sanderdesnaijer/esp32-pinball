#include "graphics.h"
#include <cmath>
#include <cstring>
#include <cstdio>

static const uint8_t FONT_4X6[][4] = {
  {0x3E,0x41,0x41,0x3E}, // 0
  {0x00,0x42,0x7F,0x40}, // 1
  {0x62,0x51,0x49,0x46}, // 2
  {0x22,0x49,0x49,0x36}, // 3
  {0x0F,0x08,0x08,0x7F}, // 4
  {0x4F,0x49,0x49,0x31}, // 5
  {0x3E,0x49,0x49,0x32}, // 6
  {0x01,0x71,0x09,0x07}, // 7
  {0x36,0x49,0x49,0x36}, // 8
  {0x26,0x49,0x49,0x3E}, // 9
  {0x7E,0x09,0x09,0x7E}, // A (10)
  {0x7F,0x49,0x49,0x36}, // B
  {0x3E,0x41,0x41,0x22}, // C
  {0x7F,0x41,0x41,0x3E}, // D
  {0x7F,0x49,0x49,0x41}, // E
  {0x7F,0x09,0x09,0x01}, // F
  {0x3E,0x41,0x49,0x3A}, // G
  {0x7F,0x08,0x08,0x7F}, // H
  {0x00,0x41,0x7F,0x41}, // I
  {0x20,0x40,0x40,0x3F}, // J
  {0x7F,0x08,0x14,0x63}, // K
  {0x7F,0x40,0x40,0x40}, // L
  {0x7F,0x02,0x02,0x7F}, // M
  {0x7F,0x04,0x08,0x7F}, // N
  {0x3E,0x41,0x41,0x3E}, // O
  {0x7F,0x09,0x09,0x06}, // P
  {0x3E,0x41,0x61,0x7E}, // Q
  {0x7F,0x09,0x19,0x66}, // R
  {0x26,0x49,0x49,0x32}, // S
  {0x01,0x01,0x7F,0x01}, // T (29 = index 29)
  {0x3F,0x40,0x40,0x3F}, // U
  {0x1F,0x20,0x40,0x3F}, // V
  {0x7F,0x20,0x20,0x7F}, // W
  {0x63,0x14,0x08,0x63}, // X
  {0x03,0x04,0x78,0x07}, // Y
  {0x61,0x51,0x49,0x47}, // Z
  {0x00,0x00,0x00,0x00}, // space (36)
  {0x00,0x00,0x60,0x00}, // . (37)
  {0x08,0x08,0x08,0x08}, // - (38)
  {0x00,0x36,0x36,0x00}, // : (39)
  {0x00,0x00,0x40,0x00}, // ! (40)
};

Graphics::Graphics() {
  clear();
}

void Graphics::clear() {
  memset(buffer, 0, BUFFER_SIZE);
}

bool Graphics::inBounds(int x, int y) const {
  return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

void Graphics::setPixel(int x, int y, bool on) {
  if (!inBounds(x, y)) return;

  int byteIdx = (y * WIDTH + x) / 8;
  int bitIdx = (x) % 8;

  if (on) {
    buffer[byteIdx] |= (1 << bitIdx);
  } else {
    buffer[byteIdx] &= ~(1 << bitIdx);
  }
}

bool Graphics::getPixel(int x, int y) const {
  if (!inBounds(x, y)) return false;

  int byteIdx = (y * WIDTH + x) / 8;
  int bitIdx = (x) % 8;
  return (buffer[byteIdx] >> bitIdx) & 1;
}

void Graphics::bresenham(int x0, int y0, int x1, int y1, bool on) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = x0 < x1 ? 1 : -1;
  int sy = y0 < y1 ? 1 : -1;
  int err = dx - dy;

  int x = x0, y = y0;
  while (true) {
    setPixel(x, y, on);
    if (x == x1 && y == y1) break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x += sx;
    }
    if (e2 < dx) {
      err += dx;
      y += sy;
    }
  }
}

void Graphics::drawLine(int x0, int y0, int x1, int y1, bool on) {
  bresenham(x0, y0, x1, y1, on);
}

void Graphics::drawRect(int x, int y, int w, int h, bool filled, bool on) {
  if (filled) {
    for (int row = y; row < y + h; row++) {
      for (int col = x; col < x + w; col++) {
        setPixel(col, row, on);
      }
    }
  } else {
    drawLine(x, y, x + w - 1, y, on);
    drawLine(x + w - 1, y, x + w - 1, y + h - 1, on);
    drawLine(x + w - 1, y + h - 1, x, y + h - 1, on);
    drawLine(x, y + h - 1, x, y, on);
  }
}

static int fontIndex(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
  if (c >= 'a' && c <= 'z') return c - 'a' + 10;
  if (c == ' ') return 36;
  if (c == '.') return 37;
  if (c == '-') return 38;
  if (c == ':') return 39;
  if (c == '!') return 40;
  return 36;
}

void Graphics::drawChar(int x, int y, char c, bool on) {
  int idx = fontIndex(c);
  if (idx < 0 || idx > 40) return;
  for (int col = 0; col < 4; col++) {
    uint8_t colData = FONT_4X6[idx][col];
    for (int row = 0; row < 7; row++) {
      if (colData & (1 << row)) {
        setPixel(x + col, y + row, on);
      }
    }
  }
}

void Graphics::drawString(int x, int y, const char* str, bool on) {
  while (*str) {
    drawChar(x, y, *str, on);
    x += 5;
    str++;
  }
}

void Graphics::drawStringCentered(int y, const char* str, bool on) {
  int len = strlen(str);
  int totalWidth = len * 5 - 1;
  int x = (WIDTH - totalWidth) / 2;
  drawString(x, y, str, on);
}

void Graphics::drawCircle(int cx, int cy, int r, bool filled, bool on) {
  if (filled) {
    for (int y = -r; y <= r; y++) {
      for (int x = -r; x <= r; x++) {
        if (x * x + y * y <= r * r) {
          setPixel(cx + x, cy + y, on);
        }
      }
    }
  } else {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (x <= y) {
      setPixel(cx + x, cy + y, on);
      setPixel(cx - x, cy + y, on);
      setPixel(cx + x, cy - y, on);
      setPixel(cx - x, cy - y, on);
      setPixel(cx + y, cy + x, on);
      setPixel(cx - y, cy + x, on);
      setPixel(cx + y, cy - x, on);
      setPixel(cx - y, cy - x, on);

      if (d < 0) {
        d = d + 4 * x + 6;
      } else {
        d = d + 4 * (x - y) + 10;
        y--;
      }
      x++;
    }
  }
}
