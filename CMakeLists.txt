cmake_minimum_required(VERSION 3.10.0)
project(dns-server VERSION 0.1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags for warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Recursively find all .cpp files in the src directory and its subdirectories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "include/*.h*")

# Tell CMake where to find header files
include_directories(src include)

add_executable(${PROJECT_NAME} ${SOURCES})

# Set the output directory for the executable (e.g., build/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
