cmake_minimum_required(VERSION 3.3)
project(openmp)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c)
add_executable(openmp ${SOURCE_FILES})