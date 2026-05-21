#include "ball.h"
#include "graphics.h"
#include <cmath>

Ball::Ball(float startX, float startY) : x(startX), y(startY), velX(0), velY(0) {}

void Ball::applyPhysics(float deltaTime) {
  velY += GRAVITY * deltaTime;

  float f = 1.0f - (1.0f - FRICTION) * deltaTime * 60.0f;
  if (f < 0.9f) f = 0.9f;
  velX *= f;
  velY *= f;

  float speed = sqrtf(velX * velX + velY * velY);
  if (speed > MAX_SPEED && speed > 0.001f) {
    float scale = MAX_SPEED / speed;
    velX *= scale;
    velY *= scale;
  }
}

int Ball::getSubSteps(float deltaTime) const {
  float speed = sqrtf(velX * velX + velY * velY);
  int steps = static_cast<int>(speed * deltaTime / 1.0f) + 1;
  if (steps > 10) steps = 10;
  return steps;
}

void Ball::step(float subDt) {
  x += velX * subDt;
  y += velY * subDt;

  if (x - RADIUS < 0) {
    x = RADIUS;
    velX = -velX * BOUNCE_DAMPING;
  }
  if (x + RADIUS > Graphics::WIDTH) {
    x = Graphics::WIDTH - RADIUS;
    velX = -velX * BOUNCE_DAMPING;
  }

  if (y - RADIUS < 0) {
    y = RADIUS;
    velY = -velY * BOUNCE_DAMPING;
  }
}

void Ball::draw(Graphics& g) {
  int ix = static_cast<int>(x);
  int iy = static_cast<int>(y);
  g.drawCircle(ix, iy, 2, true, true);
}

bool Ball::isOutOfBounds() const {
  return y > Graphics::HEIGHT;
}

void Ball::reset(float startX, float startY) {
  x = startX;
  y = startY;
  velX = 0;
  velY = 0;
}
