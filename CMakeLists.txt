cmake_minimum_required(VERSION 3.10)

# Project Name
project(ABC_SALES_OBJECTIVE_APPROXIMAT)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add executable
add_executable(ABC_SALES_OBJECTIVE_APPROXIMAT ${SOURCES})

# Link pthread
target_link_libraries(ABC_SALES_OBJECTIVE_APPROXIMAT pthread)