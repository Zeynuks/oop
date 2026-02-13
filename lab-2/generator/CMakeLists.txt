project(PrimeGenerator)

enable_testing()

add_library(prime_generator_lib prime_generator.cpp prime_generator.h)

add_executable(prime_generator main.cpp)
target_link_libraries(prime_generator prime_generator_lib)

add_subdirectory(tests)
add_subdirectory(benchmark)