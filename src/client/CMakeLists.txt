cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_client)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wall -Wunknown-pragmas")

file(GLOB_RECURSE SOURCE ./*.c ./*.h)
file(GLOB_RECURSE UTILS ../utils/*.c ../utils/*.h)

add_executable(client
        ${SOURCE}
        ${UTILS}
        ../../lib/argparser/src/argparser.c
        ../../lib/argparser/src/argparser.h
        )
