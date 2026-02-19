add_executable(tests tests.cpp)

target_link_libraries(tests gtest_main numbers_lib)

add_test(NAME VectorTests COMMAND tests)
