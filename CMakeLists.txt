# CMakeLists.txt para tests
cmake_minimum_required(VERSION 3.10)

project(ABC_SALES_OBJECTIVE_APPROXIMAT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(ABC_SALES_OBJECTIVE_APPROXIMAT ${SOURCES})

add_subdirectory(tests)
