cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_server)

find_package(Threads)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wall")

add_executable(server
        main.c
        server_connection.c
        server_connection.h
        ../debug.h
        database/database_actions.c
        database/database_actions.h
        database/resources_allocator/key_value_database_allocator.c
        database/resources_allocator/key_value_database_allocator.h
        database/resources_allocator/stack/stack.c
        database/resources_allocator/stack/stack.h
        ../data_structures/parallel/lock_free_set/lock_free_set.c
        ../data_structures/parallel/lock_free_set/lock_free_set_hash.c
        database/key_value_database_typedef.h
        tcp_shell.c
        tcp_shell.h
        database/key_value_database_typedef.c
        ../data_structures/dequeue/dequeue.c
        )

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})
