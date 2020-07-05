cmake_minimum_required(VERSION 3.10)
project(ve280_lab7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

add_executable(ve280_lab7 test/simpletest.cpp course.cpp course.h)
