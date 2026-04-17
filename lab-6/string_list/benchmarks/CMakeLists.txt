add_executable(MyListBenchmark MyListBenchmarks.cpp)

target_link_libraries(MyListBenchmark
        benchmark::benchmark
)