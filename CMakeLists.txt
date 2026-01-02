cmake_minimum_required(VERSION 3.10.0)
project(dns-server VERSION 0.1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags for warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Enable testing
enable_testing()

# --- dnslib ---
# Add the library subdirectory
add_subdirectory(lib/dnslib)

# --- Executable ---
file(GLOB APP_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/utils/*.cpp")
add_executable(${PROJECT_NAME} ${APP_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE dnslib)


# Set the output directory for the executable (e.g., build/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

