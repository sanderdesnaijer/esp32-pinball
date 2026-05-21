#pragma once

#include <cstdint>

// Input handler for capacitive touch pads and optional debug buttons
class InputHandler {
public:
  InputHandler();

  void initialize();
  void update();

  bool isLeftFlipperPressed() const { return leftFlipperActive; }
  bool isRightFlipperPressed() const { return rightFlipperActive; }
  bool isAnyPressed() const { return leftFlipperActive || rightFlipperActive; }

private:
  bool leftFlipperActive;
  bool rightFlipperActive;

  // Touch thresholds (tuning required based on hardware)
  // Lower values = more sensitive. Start at 40 and adjust.
  static constexpr uint16_t TOUCH_THRESHOLD = 800;

  void readTouchPads();
};
