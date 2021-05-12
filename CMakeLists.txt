cmake_minimum_required(VERSION 3.17)
project(t1 C)

set(CMAKE_C_STANDARD 99)

add_executable(t1 src/main.c src/vehicle.c src/vehicle.h src/helpers.c src/helpers.h src/line.c src/line.h src/handlers.c src/handlers.h)