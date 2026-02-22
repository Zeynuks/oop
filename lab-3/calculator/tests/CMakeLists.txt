add_executable(calculator_tests tests.cpp)

target_link_libraries(calculator_tests gtest_main calculator_lib)

add_test(NAME CalculatorTests COMMAND calculator_tests)