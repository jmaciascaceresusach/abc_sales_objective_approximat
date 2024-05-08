cmake_minimum_required(VERSION 3.10)

project(ABC_SALES_OBJECTIVE_APPROXIMAT_TESTS)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests ABC_SALES_OBJECTIVE_APPROXIMAT)
