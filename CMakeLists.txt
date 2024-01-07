cmake_minimum_required(VERSION 3.25)
project(result)

set(CMAKE_CXX_STANDARD 17)
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

add_custom_command(
  OUTPUT result.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND python3 -m venv venv
  COMMAND source venv/bin/activate
  COMMAND pip3 install quom
  COMMAND quom src/result-wrapper.h result.h
  COMMENT "Merging source files into single header"
  VERBATIM
)

add_custom_target(
  result_header ALL
  DEPENDS result.h
)

file(GLOB_RECURSE TESTS_SOURCES "tests/*.cpp")
add_executable(
  tests
  ${TESTS_SOURCES}
)
add_dependencies(tests result_header)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)