#pragma once

class Ball;
class Graphics;

// Bumper: stationary obstacle that bounces the ball and adds score
class Bumper {
public:
  Bumper(float cx, float cy, float radius);

  void draw(Graphics& g);
  void update(float deltaTime);
  bool checkCollision(Ball& ball);

  float getCenterX() const { return cx; }
  float getCenterY() const { return cy; }

  int getScore() const { return score; }

private:
  float cx, cy;
  float radius;
  int score;
  float hitTimer;
  static constexpr int SCORE_VALUE = 100;
  static constexpr float HIT_FLASH_TIME = 0.15f;
};
