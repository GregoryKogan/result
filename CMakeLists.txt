cmake_minimum_required(VERSION 3.25)
project(result)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

file(GLOB_RECURSE SOURCES "src/**/*.cpp" "src/**/*.hpp")
file(GLOB_RECURSE TESTS_SOURCES "tests/*.cpp")

add_executable(
  tests
  ${SOURCES}
  ${TESTS_SOURCES}
)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)