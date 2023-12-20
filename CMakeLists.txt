cmake_minimum_required(VERSION 3.25)
project(result)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/**/*.cpp" "src/**/*.hpp")
add_executable(
    result
    src/main.cpp
    ${SOURCES}
)