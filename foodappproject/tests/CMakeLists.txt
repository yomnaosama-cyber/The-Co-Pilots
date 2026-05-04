# tests/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# Find Qt6 with all required components for database.cpp
find_package(Qt6 REQUIRED COMPONENTS Core Sql Widgets)

enable_testing()

# Fetch GoogleTest and gMock
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# ============================================
# 1. Unit Tests (Database)
# ============================================
add_executable(db_tests 
    test_database.cpp
    ../database.cpp
)

target_link_libraries(db_tests PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Sql
    Qt6::Widgets
)

target_include_directories(db_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

# Set C++ standard for tests
set_target_properties(db_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ============================================
# 2. Address Matcher Tests (Integration)
# ============================================
add_executable(matcher_tests
    test_address_matcher.cpp
    ../database.cpp
)

target_link_libraries(matcher_tests PRIVATE
    gtest_main
    Qt6::Core
    Qt6::Sql
    Qt6::Widgets
)

target_include_directories(matcher_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

set_target_properties(matcher_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ============================================
# 3. Mock Tests (Isolated Business Logic)
# ============================================
add_executable(mock_tests
    test_mocks.cpp
)

target_link_libraries(mock_tests PRIVATE
    gtest_main
    gmock
)

target_include_directories(mock_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}
)

set_target_properties(mock_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# ============================================
# Discover all tests for CTest
# ============================================
include(GoogleTest)
gtest_discover_tests(db_tests)
gtest_discover_tests(matcher_tests)
gtest_discover_tests(mock_tests)