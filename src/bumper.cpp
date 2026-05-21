#include "bumper.h"
#include "ball.h"
#include "graphics.h"
#include <cmath>

Bumper::Bumper(float cx, float cy, float radius)
    : cx(cx), cy(cy), radius(radius), score(0), hitTimer(0) {}

void Bumper::update(float deltaTime) {
  if (hitTimer > 0) hitTimer -= deltaTime;
}

void Bumper::draw(Graphics& g) {
  int icx = static_cast<int>(cx);
  int icy = static_cast<int>(cy);
  bool filled = hitTimer > 0;
  g.drawCircle(icx, icy, static_cast<int>(radius), filled, true);
}

bool Bumper::checkCollision(Ball& ball) {
  float bx = ball.getX();
  float by = ball.getY();

  float dx = bx - cx;
  float dy = by - cy;
  float dist = sqrt(dx * dx + dy * dy);

  if (dist >= radius + Ball::COLLISION_RADIUS) {
    return false;
  }

  if (dist < 0.1f) dist = 0.1f;
  float nx = dx / dist;
  float ny = dy / dist;

  float overlap = radius + Ball::COLLISION_RADIUS - dist;
  ball.setPosition(bx + nx * overlap, by + ny * overlap);

  float vx = ball.getVelX();
  float vy = ball.getVelY();
  float velDotN = vx * nx + vy * ny;
  if (velDotN < 0) {
    ball.setVelocity(vx - 2.0f * velDotN * nx, vy - 2.0f * velDotN * ny);
  }
  ball.applyForce(nx * 10.0f, ny * 10.0f);

  hitTimer = HIT_FLASH_TIME;
  score = SCORE_VALUE;
  return true;
}
