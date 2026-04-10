add_executable(my_list_tests
        MyListConstructorTests.cpp
        MyListAssignmentTests.cpp)

target_link_libraries(my_list_tests
        gtest_main
        gmock
)

add_test(NAME MyListTests COMMAND my_list_tests)