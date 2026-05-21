# Arduino / PlatformIO Setup

## Hardware Wiring

**OLED Display (SSD1306, I²C)**
- VCC → 3.3V
- GND → GND
- SCL → GPIO 22
- SDA → GPIO 21

<p align="center">
  <img src="docs/esp32-ssd1306-wiring-diagram.webp" alt="Wiring diagram: ESP32 DevKit to SSD1306 OLED over I2C. SDA on GPIO 21, SCL on GPIO 22, VCC to 3.3V, GND to GND." width="500"/>
</p>

**Capacitive Touch Pads**

| Flipper | Touch Pads | GPIOs |
|---|---|---|
| Left | T8, T9 | GPIO 33, GPIO 32 |
| Right | T4, T5, T6, T7 | GPIO 4, GPIO 0, GPIO 2, GPIO 27 |

The ESP32's touch pad sensors work by proximity. No button needed, just a bare pad or short wire soldered to the pin. Place pads near the physical flippers for easy access.

<!-- TODO: Add a wiring diagram for the touch pad connections -->

## PlatformIO

### Install

1. Install VS Code extension: **PlatformIO IDE**
2. PlatformIO CLI will auto-install on first use

### Build & Upload

```bash
cd esp32-pinball

# Build
platformio run

# Upload to board (auto-detects COM port)
platformio run -t upload

# Monitor serial output
platformio device monitor --baud 115200
```

### Troubleshooting

- **Board not detected**: Check USB cable, try different USB port
- **Compilation fails**: Run `platformio lib install` to fetch dependencies
- **Touch not responding**: Threshold may need tuning — see `input_handler.h` `TOUCH_THRESHOLD`

## Arduino IDE Alternative

1. Open Arduino IDE
2. File → Preferences → Additional Boards Manager URLs:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Tools → Board Manager → Search "esp32" → Install latest
4. Select Tools → Board → "ESP32 Dev Module"
5. Create a new sketch, copy `src/main.cpp` (Arduino section only)
6. Add all other `.cpp` and `.h` files as tabs
7. Sketch → Include Library → Manage Libraries → Install:
   - Adafruit SSD1306
   - Adafruit GFX Library
8. Upload

## Display Orientation

The code uses logical coordinates (64×128, portrait). The OLED driver (`Adafruit_SSD1306`) automatically maps this to the physical 128×64 display. Mount the OLED vertically so the long edge is vertical.

## Testing on Desktop

To test the game engine locally (before flashing):

```bash
cd build
cmake ..
make
./pinball_test
```

This runs without Arduino libraries, displays in the console.
