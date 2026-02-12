project(replace)

enable_testing()

add_library(replace_core replace_core.cpp replace_core.h)

add_executable(replace replace.cpp)
target_link_libraries(replace replace_core)

add_subdirectory(benchmark)