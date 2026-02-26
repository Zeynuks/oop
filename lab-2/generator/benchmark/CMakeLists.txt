add_executable(prime_generator_benchmark benchmark.cpp)

target_link_libraries(prime_generator_benchmark
        prime_generator_lib
        benchmark::benchmark
)