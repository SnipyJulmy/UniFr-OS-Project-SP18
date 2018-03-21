cmake_minimum_required(VERSION 3.9)
project(${CMAKE_PROJECT_NAME}_test)

set(CTEST_DIR ${CMAKE_SOURCE_DIR}/test/lib/ctest)


set(CTEST_DIR ${CMAKE_SOURCE_DIR}/test/lib/ctest)
if(NOT EXISTS ${CTEST_DIR})
    find_package(Git REQUIRED)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} clone https://github.com/bvdberg/ctest.git ${CTEST_DIR}
    )
endif()

add_subdirectory(unit_test)