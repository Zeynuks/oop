project(Calculator)

enable_testing()

add_executable(Calculator main.cpp)

add_subdirectory(core)
add_subdirectory(tests)