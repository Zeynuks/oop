add_executable(my_string_tests
        MyStringConstructorsTests.cpp
        MyStringAssignmentAndSubstrTests.cpp
        MyStringArithmeticTest.cpp
        MyStringComparisonTest.cpp
        MyStringIteratorTest.cpp)

target_link_libraries(my_string_tests
        gtest_main
        gmock
)

add_test(NAME MyStringTests COMMAND my_string_tests)