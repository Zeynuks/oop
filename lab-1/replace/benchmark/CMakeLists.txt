add_executable(replace_benchmark benchmark.cpp)

target_link_libraries(replace_benchmark
        replace_core
        benchmark::benchmark
)