cmake_minimum_required(VERSION 3.22)
project(CPractice C)

set(CMAKE_C_STANDARD 99)

# Include all source files necessary for the executable.
# Ensure that both stack.c and main.c are compiled into the executable.
add_executable(CPractice tests.c Stack.c)

