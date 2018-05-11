if (UNIX)
    link_libraries(m)
endif (UNIX)

find_package(OpenMP)
if (OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -std=c11 -Wno-unused-variable -Wno-pointer-to-int-cast -Wno-unknown-pragmas")

file(GLOB_RECURSE "TEST_SOURCE" ./*.c ./*.h)
file(GLOB_RECURSE "SOURCE"
        ../../src/server/database/*.c
        ../../src/server/database/*.h
        ../../src/data_structures/*.c
        ../../src/data_structures/*.h
        )

add_executable(Unit_Tests_run
        ${TEST_SOURCE}
        ${SOURCE}
        )
