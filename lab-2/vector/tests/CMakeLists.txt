add_executable(vector_tests tests.cpp)

target_link_libraries(vector_tests gtest_main numbers_lib)

add_test(NAME VectorTests COMMAND vector_tests)
