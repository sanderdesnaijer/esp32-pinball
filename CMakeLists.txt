cmake_minimum_required(VERSION 3.10)
project(esp32-pinball)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pinball_core
  src/graphics.cpp
  src/ball.cpp
  src/flipper.cpp
  src/bumper.cpp
  src/input_handler.cpp
  src/game_engine.cpp
  src/wall.cpp
)

target_include_directories(pinball_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(pinball_test src/main.cpp)
target_link_libraries(pinball_test pinball_core)
