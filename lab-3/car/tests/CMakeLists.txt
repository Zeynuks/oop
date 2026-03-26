add_executable(car_tests tests.cpp)

target_link_libraries(car_tests gtest_main car_lib)

add_test(NAME CarTests COMMAND car_tests)