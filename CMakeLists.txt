cmake_minimum_required(VERSION 3.14)
project(console_array_poc C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000000")

add_executable(console_array_poc main.c)