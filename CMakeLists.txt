cmake_minimum_required(VERSION 3.0)
project(Snake_minigame)

add_executable(Main src/main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/libs)

set_property(TARGET Main PROPERTY CXX_STANDARD 17)
set_property(TARGET Main PROPERTY CXX_STANDARD_REQUIRED ON)


