add_executable(AhoCorasickBenchmark benchmark.cpp)

target_link_libraries(AhoCorasickBenchmark
        aho_corasick_lib
        benchmark::benchmark
)