add_executable(MyStringBenchmark MyStringBenchmarks.cpp)

target_link_libraries(MyStringBenchmark
        benchmark::benchmark
)