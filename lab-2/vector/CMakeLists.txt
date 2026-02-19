project(vector)

enable_testing()

add_library(numbers_lib numbers.cpp numbers.h)

add_executable(vector main.cpp)
target_link_libraries(vector numbers_lib)

add_subdirectory(tests)