cmake_minimum_required(VERSION 3.9)

project(key_value_database C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wall -Wunknown-pragmas")

# import argparser
set(ARGPARSER_DIR ${CMAKE_SOURCE_DIR}/lib/argparser)
if (NOT EXISTS ${ARGPARSER_DIR})
    find_package(Git REQUIRED)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/maandree/argparser.git ${ARGPARSER_DIR}
    )
endif ()

add_subdirectory(src/client)
add_subdirectory(src/server)
add_subdirectory(test)