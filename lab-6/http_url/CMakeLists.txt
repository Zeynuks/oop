project(HttpUrl)

enable_testing()

add_executable(HttpUrl main.cpp HttpUrl.hpp)

add_subdirectory(tests)