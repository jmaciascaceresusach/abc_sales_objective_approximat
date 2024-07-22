cmake_minimum_required(VERSION 3.10)
project(ABC_SALES_OBJECTIVE_APPROXIMAT)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(ABC_SALES_OBJECTIVE_APPROXIMAT 
    src/main.cpp
    src/ABCMethod.cpp
    src/Parameter.cpp
    src/SimulationEngine.cpp
    src/DataLoader.cpp
)

target_link_libraries(ABC_SALES_OBJECTIVE_APPROXIMAT pthread)