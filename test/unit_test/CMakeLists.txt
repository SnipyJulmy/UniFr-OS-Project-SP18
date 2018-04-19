include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

if (UNIX)
    link_libraries(m)
endif (UNIX)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wall -Wuninitialized")

add_executable(Unit_Tests_run
        main_test.c
        multi_thread_access_test.c
        reverse_test.c
        set_semantics_test.c
        database_actions_tests.c
        data_structures_test/dequeue_test.c

        ../../src/server/server_connection.c
        ../../src/server/database/database_actions.c
        ../../src/data_structures/parallel/lock_free_set/lock_free_set.c
        ../../src/data_structures/parallel/lock_free_set/lock_free_set_hash.c
        ../../src/server/database/resources_allocator/key_value_database_allocator.c
        ../../src/server/tcp_shell.c
        ../../src/data_structures/dequeue/dequeue.c
        ../../src/server/database/key_value_database_typedef.c)
