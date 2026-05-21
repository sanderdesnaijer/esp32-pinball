#pragma once

#include "graphics.h"
#include "ball.h"
#include "flipper.h"
#include "bumper.h"
#include "wall.h"
#include "input_handler.h"
#include <vector>

enum class GameState { TITLE, PLAYING, GAME_OVER };

class GameEngine {
public:
  GameEngine();

  void initialize();
  void update(float deltaTime);
  void render();

  const Graphics& getGraphics() const { return graphics; }
  int getScore() const { return score; }
  int getBallsRemaining() const { return ballsRemaining; }
  GameState getState() const { return state; }

private:
  Graphics graphics;
  Ball ball;
  Flipper leftFlipper;
  Flipper rightFlipper;
  std::vector<Bumper> bumpers;
  std::vector<Wall> walls;
  InputHandler input;

  GameState state;
  int score;
  int ballsRemaining;
  bool prevAnyPressed;

  void setupLayout();
  void handleCollisions();
  void resetGame();
  void spawnBall();
};
