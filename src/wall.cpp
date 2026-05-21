#include "wall.h"
#include "ball.h"
#include "graphics.h"
#include <cmath>

Wall::Wall(float x0, float y0, float x1, float y1)
    : x0(x0), y0(y0), x1(x1), y1(y1) {}

void Wall::draw(Graphics& g) {
  int ix0 = static_cast<int>(x0);
  int iy0 = static_cast<int>(y0);
  int ix1 = static_cast<int>(x1);
  int iy1 = static_cast<int>(y1);
  g.drawLine(ix0, iy0, ix1, iy1, true);
  float dx = x1 - x0;
  float dy = y1 - y0;
  float len = sqrtf(dx * dx + dy * dy);
  if (len < 0.1f) return;
  int nx = static_cast<int>(-dy / len);
  int ny = static_cast<int>(dx / len);
  g.drawLine(ix0 + nx, iy0 + ny, ix1 + nx, iy1 + ny, true);
}

void Wall::checkCollision(Ball& ball) {
  float bx = ball.getX();
  float by = ball.getY();
  // Wider collision margin than Ball::COLLISION_RADIUS to prevent
  // tunneling through thin guide walls at high speed.
  static constexpr float WALL_COLLISION_MARGIN = 3.0f;

  float dx = x1 - x0;
  float dy = y1 - y0;
  float len2 = dx * dx + dy * dy;
  if (len2 < 0.001f) return;

  float t = ((bx - x0) * dx + (by - y0) * dy) / len2;
  if (t < 0) t = 0;
  if (t > 1) t = 1;

  float cx = x0 + t * dx;
  float cy = y0 + t * dy;

  float distX = bx - cx;
  float distY = by - cy;
  float dist = sqrtf(distX * distX + distY * distY);

  if (dist >= WALL_COLLISION_MARGIN) return;
  if (dist < 0.01f) dist = 0.01f;

  float nx = distX / dist;
  float ny = distY / dist;

  float overlap = WALL_COLLISION_MARGIN - dist;
  ball.setPosition(bx + nx * overlap, by + ny * overlap);

  float vx = ball.getVelX();
  float vy = ball.getVelY();
  float velDotN = vx * nx + vy * ny;
  if (velDotN < 0) {
    ball.setVelocity((vx - 2.0f * velDotN * nx) * BOUNCE_DAMPING,
                     (vy - 2.0f * velDotN * ny) * BOUNCE_DAMPING);
  }
}
