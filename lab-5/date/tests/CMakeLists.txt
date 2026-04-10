add_executable(my_date_tests tests.cpp)

target_link_libraries(my_date_tests gtest_main)

add_test(NAME MyDateTests COMMAND my_date_tests)