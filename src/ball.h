#pragma once

// Fixed-point ball physics; all values in game coordinates
// x ∈ [0, 63], y ∈ [0, 127]
class Ball {
public:
  Ball(float startX, float startY);

  void applyPhysics(float deltaTime);
  void step(float subDt);
  int getSubSteps(float deltaTime) const;
  void draw(class Graphics& g);

  float getX() const { return x; }
  float getY() const { return y; }
  float getVelX() const { return velX; }
  float getVelY() const { return velY; }

  void setVelocity(float vx, float vy) {
    velX = vx;
    velY = vy;
  }

  void applyForce(float fx, float fy) {
    velX += fx;
    velY += fy;
  }

  void setPosition(float px, float py) { x = px; y = py; }

  bool isOutOfBounds() const;
  void reset(float startX, float startY);

  // Collision radius used by other game objects (flippers, bumpers).
  // Larger than RADIUS to give a forgiving collision zone.
  static constexpr float COLLISION_RADIUS = 1.5f;

private:
  float x, y;
  float velX, velY;
  static constexpr float RADIUS = 1.0f;
  static constexpr float GRAVITY = 50.0f;
  static constexpr float FRICTION = 0.999f;
  static constexpr float BOUNCE_DAMPING = 0.85f;
  static constexpr float MAX_SPEED = 80.0f;
};
