add_executable(calculator_tests tests.cpp)

target_link_libraries(calculator_tests gtest_main)

add_test(NAME CalculatorTests COMMAND calculator_tests)