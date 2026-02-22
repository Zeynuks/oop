project(Calculator)

enable_testing()

add_library(calculator_lib
        Calculator.cpp
        Calculator.hpp
)

add_executable(Calculator main.cpp)
target_link_libraries(Calculator calculator_lib)

add_subdirectory(tests)