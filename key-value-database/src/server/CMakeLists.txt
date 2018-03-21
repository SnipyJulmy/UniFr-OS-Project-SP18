cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_server)

set(CMAKE_C_STANDARD 11)

add_executable(server
        main.c
        )
