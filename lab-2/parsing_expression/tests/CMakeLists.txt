add_executable(tests tests.cpp)

target_link_libraries(tests gtest_main parsing_expression_lib)

add_test(NAME ParsingExpressionTests COMMAND tests)