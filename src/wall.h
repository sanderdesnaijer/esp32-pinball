#pragma once

class Ball;
class Graphics;

class Wall {
public:
  Wall(float x0, float y0, float x1, float y1);

  void draw(Graphics& g);
  void checkCollision(Ball& ball);

private:
  float x0, y0, x1, y1;
  static constexpr float BOUNCE_DAMPING = 0.8f;
};
