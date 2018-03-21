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
        )
