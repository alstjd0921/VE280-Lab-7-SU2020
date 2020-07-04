cmake_minimum_required(VERSION 3.16)
project(ve280_lab7)

set(CMAKE_CXX_STANDARD 17)

# add_executable(ve280_lab7 main.cpp course.cpp course.h)
add_executable(ve280_lab7 test/simpletest.cpp course.cpp course.h)