add_executable(array_tests
        MyArrayConstructorsTests.cpp
        MyArrayCapacityTests.cpp
        MyArrayComparisonTests.cpp
        MyArrayElementAccessTests.cpp
        MyArrayIteratorsTests.cpp
        MyArrayModifiersTests.cpp
)

target_link_libraries(array_tests
        gtest_main
        gmock
)

add_test(NAME MyArrayTests COMMAND array_tests)