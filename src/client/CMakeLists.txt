cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_client)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wall -Wunknown-pragmas")

add_executable(client
        main.c
        ../debug.h
        shell.c
        shell.h
        )
