cmake_minimum_required(VERSION 3.12)
project(mazeGenerator)

set(CMAKE_CXX_STANDARD 11)


# Add your source files
set(SOURCES mazeGenerator.cpp)

# Add executable
add_executable(program ${SOURCES})

