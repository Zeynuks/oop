add_executable(tests tests.cpp)

target_link_libraries(tests gtest_main prime_generator_lib)

add_test(NAME PrimeGeneratorTests COMMAND tests)