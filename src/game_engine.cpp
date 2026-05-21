#include "game_engine.h"
#include <cstdlib>
#include <cstdio>

GameEngine::GameEngine()
    : ball(25.0f, 10.0f), leftFlipper(10.0f, 112.0f, 18.0f, true),
      rightFlipper(54.0f, 112.0f, 18.0f, false),
      state(GameState::TITLE), score(0), ballsRemaining(3),
      prevAnyPressed(false) {
  setupLayout();
}

void GameEngine::initialize() {
  input.initialize();
}

void GameEngine::setupLayout() {
  bumpers.emplace_back(20.0f, 30.0f, 3.0f);
  bumpers.emplace_back(44.0f, 30.0f, 3.0f);
  bumpers.emplace_back(12.0f, 50.0f, 3.0f);
  bumpers.emplace_back(32.0f, 50.0f, 3.0f);
  bumpers.emplace_back(52.0f, 50.0f, 3.0f);
  bumpers.emplace_back(20.0f, 70.0f, 3.0f);
  bumpers.emplace_back(44.0f, 70.0f, 3.0f);

  walls.emplace_back(0.0f, 88.0f, 10.0f, 112.0f);
  walls.emplace_back(63.0f, 88.0f, 54.0f, 112.0f);
}

void GameEngine::resetGame() {
  score = 0;
  ballsRemaining = 3;
  spawnBall();
}

void GameEngine::spawnBall() {
  float randomX = 10.0f + (rand() % 44);
  ball.reset(randomX, 12.0f);
  float kick = (rand() % 2 == 0) ? 5.0f : -5.0f;
  ball.applyForce(kick, 0);
}

void GameEngine::update(float deltaTime) {
  input.update();

  bool anyPressed = input.isAnyPressed();
  bool justPressed = anyPressed && !prevAnyPressed;
  prevAnyPressed = anyPressed;

  switch (state) {
    case GameState::TITLE:
      if (justPressed) {
        resetGame();
        state = GameState::PLAYING;
      }
      break;

    case GameState::PLAYING: {
      leftFlipper.setActive(input.isLeftFlipperPressed());
      rightFlipper.setActive(input.isRightFlipperPressed());

      leftFlipper.update(deltaTime);
      rightFlipper.update(deltaTime);
      for (auto& bumper : bumpers) {
        bumper.update(deltaTime);
      }

      ball.applyPhysics(deltaTime);
      int steps = ball.getSubSteps(deltaTime);
      float subDt = deltaTime / steps;
      for (int i = 0; i < steps; i++) {
        ball.step(subDt);
        handleCollisions();
      }

      if (ball.isOutOfBounds()) {
        ballsRemaining--;
        if (ballsRemaining <= 0) {
          state = GameState::GAME_OVER;
        } else {
          spawnBall();
        }
      }
      break;
    }

    case GameState::GAME_OVER:
      if (justPressed) {
        resetGame();
        state = GameState::PLAYING;
      }
      break;
  }
}

void GameEngine::handleCollisions() {
  leftFlipper.checkCollision(ball);
  rightFlipper.checkCollision(ball);

  for (auto& bumper : bumpers) {
    if (bumper.checkCollision(ball)) {
      score += bumper.getScore();
    }
  }

  for (auto& wall : walls) {
    wall.checkCollision(ball);
  }
}

void GameEngine::render() {
  graphics.clear();

  switch (state) {
    case GameState::TITLE:
      graphics.drawStringCentered(40, "PINBALL");
      graphics.drawStringCentered(55, "PRESS");
      graphics.drawStringCentered(63, "FLIP!");
      break;

    case GameState::PLAYING: {
      graphics.drawRect(0, 0, Graphics::WIDTH, Graphics::HEIGHT, false, true);

      for (auto& bumper : bumpers) {
        bumper.draw(graphics);
      }
      for (auto& wall : walls) {
        wall.draw(graphics);
      }
      leftFlipper.draw(graphics);
      rightFlipper.draw(graphics);
      ball.draw(graphics);

      char scoreBuf[10];
      snprintf(scoreBuf, sizeof(scoreBuf), "%d", score);
      graphics.drawString(3, 2, scoreBuf);

      for (int i = 0; i < ballsRemaining; i++) {
        graphics.drawCircle(55 - i * 6, 4, 2, true, true);
      }
      break;
    }

    case GameState::GAME_OVER:
      graphics.drawStringCentered(35, "GAME");
      graphics.drawStringCentered(44, "OVER");
      {
        char scoreBuf[16];
        snprintf(scoreBuf, sizeof(scoreBuf), "%d", score);
        graphics.drawStringCentered(60, scoreBuf);
      }
      break;
  }
}
