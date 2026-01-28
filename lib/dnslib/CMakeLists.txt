cmake_minimum_required(VERSION 3.10.0)
project(dnslib)

# --- dnslib ---
# Gather all library source files
file(GLOB_RECURSE DNSLIB_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Create the library
add_library(dnslib ${DNSLIB_SOURCES})

# Make include directories available to consumers of the library
target_include_directories(${PROJECT_NAME} PUBLIC include)


# --- Tests ---
option(BUILD_TESTING "Build the testing tree." ON)

if(BUILD_TESTING)
  include(FetchContent)

  # 1. Download Google Test automatically from GitHub
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
  )
  # For older CMake versions:
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # 2. Define the test executable and include all test files
  file(GLOB_RECURSE DNSLIB_TEST_SOURCES CONFIGURE_DEPENDS "tests/*.cpp")
  add_executable(dns_tests ${DNSLIB_TEST_SOURCES})

  # 3. Link your library and GTest
  target_link_libraries(dns_tests PRIVATE dnslib gtest_main)
  target_link_libraries(dnslib PUBLIC gtest)

  # 4. Enable test discovery in CTest
  include(GoogleTest)
  gtest_discover_tests(dns_tests)
endif()