# CMake build configuration for C++ modules
cmake_minimum_required(VERSION 3.16)
project(bug_killing_robot_cpp CXX)

set(CMAKE_CXX_STANDARD 20)  # keep in sync with -std in project-root .clangd
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Robot core (mediator, state machine, pathfinding) as a reusable library so
# tests/other targets can link it.
add_library(robot_core state_machine.cpp robot.cpp pathfinding.cpp)
target_include_directories(robot_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Interactive REPL driver for exercising the robot without hardware.
add_executable(robot main.cpp)
target_link_libraries(robot PRIVATE robot_core)
