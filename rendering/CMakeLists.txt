cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

set(CMAKE_CXX_STANDARD 11)

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Find OpenGL package
find_package(OpenGL REQUIRED)

set(OPENGL_glu_LIBRARY "-lGLU")

# Add include directories for SDL2 and OpenGL
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

# Add your source files
set(SOURCES renderingMulti.cpp)

# Add executable
add_executable(program ${SOURCES})

# Link against SDL2 and OpenGL libraries
target_link_libraries(program ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})
