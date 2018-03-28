cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_server)

find_package(Threads)

set(CMAKE_C_STANDARD 11)

add_executable(server
        main.c
        server_connection.c
        server_connection.h
        ../debug.h
        database_actions.c
        database_actions.h
        resources_allocator/allocator_int_box.c
        resources_allocator/allocator_int_box.h
        resources_allocator/stack/stack.c
        resources_allocator/stack/stack.h)

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
