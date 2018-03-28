include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

if (UNIX)
    link_libraries(m)
endif (UNIX)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wall -Wuninitialized")

set(SERVER_SRC )

add_executable(Unit_Tests_run
        main_test.c
        multi_thread_access_test.c
        reverse_test.c
        set_semantics_test.c
        ../../src/server/lock_free_set/lock_free_set.c
        ../../src/server/lock_free_set/lock_free_set.h
        ../../src/server/lock_free_set/lock_free_set_hash.c
        ../../src/server/lock_free_set/lock_free_set_hash.h
        )
