cmake_minimum_required(VERSION 3.17)
project(t1 C)

set(CMAKE_C_STANDARD 99)

add_executable(t1 main.c vehicle.c vehicle.h helpers.c helpers.h line.c line.h handlers.c sorting.c sorting.h handlers.h b_tree.c b_tree.h queue.c queue.h)