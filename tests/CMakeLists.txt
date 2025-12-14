include(FetchContent)

# 1. Pobierz Google Test automatycznie z GitHuba
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For older CMake versions:
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 2. Zdefiniuj plik wykonywalny testów
add_executable(dns_tests
    main_test.cpp
    HeaderTests.cpp
    ParserTests.cpp
)

# 3. Podlinkuj Twoją bibliotekę oraz GTest
# Zakładam, że w głównym CMake nazwałeś swoją bibliotekę 'dns_lib'
target_link_libraries(dns_tests PRIVATE dnslib gtest_main)

# 4. Włącz obsługę testów w CTest
include(GoogleTest)
gtest_discover_tests(dns_tests)