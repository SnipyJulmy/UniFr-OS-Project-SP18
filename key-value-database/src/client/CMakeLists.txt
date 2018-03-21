cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_client)

set(CMAKE_C_STANDARD 11)

add_executable(client
        main.c
        )
