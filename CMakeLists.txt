cmake_minimum_required(VERSION 3.16)
project(HttpServer C)

# Set C standard (optional, but recommended)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add your executable
add_executable(HttpServer src/main.c
    src/connection.c
    src/connection.h
    src/utils.h
    src/utils.c)