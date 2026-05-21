#pragma once

class Ball;
class Graphics;

// Flipper: rotating rod that launches the ball
// Pivot at (pivotX, pivotY), extends to (endX, endY) when active
class Flipper {
public:
  Flipper(float pivotX, float pivotY, float length, bool isLeft);

  void setActive(bool active) { this->active = active; }
  bool isActive() const { return active; }

  void update(float deltaTime);
  void draw(Graphics& g);

  // Check collision with ball and apply impulse
  bool checkCollision(Ball& ball);

private:
  float pivotX, pivotY;
  float length;
  bool isLeft;
  bool active;
  float rotation;
  float rotationSpeed;
  float baseAngle;

  static constexpr float SWING_ANGLE = 0.5f;
  static constexpr float ROTATION_SPEED = 15.0f;
  static constexpr float FLIPPER_RADIUS = 2.5f;

  float getEndX() const;
  float getEndY() const;
};
