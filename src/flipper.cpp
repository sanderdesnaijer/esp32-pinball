#include "flipper.h"
#include "ball.h"
#include "graphics.h"
#include <cmath>

Flipper::Flipper(float pivotX, float pivotY, float length, bool isLeft)
    : pivotX(pivotX), pivotY(pivotY), length(length), isLeft(isLeft), active(false),
      rotation(0), rotationSpeed(0),
      baseAngle(isLeft ? 0.5f : (M_PI - 0.5f)) {}

float Flipper::getEndX() const {
  float angle = isLeft ? (baseAngle - rotation) : (baseAngle + rotation);
  return pivotX + length * cosf(angle);
}

float Flipper::getEndY() const {
  float angle = isLeft ? (baseAngle - rotation) : (baseAngle + rotation);
  return pivotY + length * sinf(angle);
}

void Flipper::update(float deltaTime) {
  float targetRot = active ? SWING_ANGLE : 0;
  rotationSpeed = (targetRot - rotation) * ROTATION_SPEED;
  rotation += rotationSpeed * deltaTime;

  if (rotation < 0) rotation = 0;
  if (rotation > SWING_ANGLE) rotation = SWING_ANGLE;
}

void Flipper::draw(Graphics& g) {
  int px = static_cast<int>(pivotX);
  int py = static_cast<int>(pivotY);
  int ex = static_cast<int>(getEndX());
  int ey = static_cast<int>(getEndY());

  g.drawLine(px, py, ex, ey, true);
  g.drawCircle(px, py, 2, true, true);
}

bool Flipper::checkCollision(Ball& ball) {
  float bx = ball.getX();
  float by = ball.getY();

  // Vector from pivot to ball
  float dx = bx - pivotX;
  float dy = by - pivotY;

  // Vector along flipper
  float ex = getEndX() - pivotX;
  float ey = getEndY() - pivotY;

  // Project ball onto flipper line
  float len2 = ex * ex + ey * ey;
  if (len2 < 0.001f) return false;

  float t = (dx * ex + dy * ey) / len2;
  t = fmax(0, fmin(1, t));

  // Closest point on flipper
  float cx = pivotX + t * ex;
  float cy = pivotY + t * ey;

  // Distance from ball to flipper
  float dist = sqrt((bx - cx) * (bx - cx) + (by - cy) * (by - cy));

  if (dist > FLIPPER_RADIUS + Ball::COLLISION_RADIUS) return false;

  if (dist < 0.01f) dist = 0.01f;
  float nx = (bx - cx) / dist;
  float ny = (by - cy) / dist;

  float overlap = FLIPPER_RADIUS + Ball::COLLISION_RADIUS - dist;
  ball.setPosition(bx + nx * overlap, by + ny * overlap);

  float vx = ball.getVelX();
  float vy = ball.getVelY();
  float velDotN = vx * nx + vy * ny;
  if (velDotN < 0) {
    ball.setVelocity(vx - 2.0f * velDotN * nx, vy - 2.0f * velDotN * ny);
  }

  if (active) {
    float impulseStrength = 25.0f;
    float kick = isLeft ? 0.3f : -0.3f;
    ball.applyForce(kick * impulseStrength, -impulseStrength);
  }

  return true;
}
