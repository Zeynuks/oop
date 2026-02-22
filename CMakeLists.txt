cmake_minimum_required(VERSION 3.20)
project(oop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
add_subdirectory(extern/benchmark)

add_subdirectory(lab-1)
add_subdirectory(lab-3)
