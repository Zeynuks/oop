add_executable(primes_generator tests.cpp)

target_link_libraries(tests gtest_main primes_generator_lib)

add_test(NAME PrimeGeneratorTests COMMAND primes_generator)