add_executable(my_list_tests
        MyListConstructorTests.cpp
        MyListAccessAndSwapTests.cpp
        MyListAdditionalTypesTests.cpp
        MyListExceptionsAndBoundaryTests.cpp
        MyListIteratorTests.cpp
        MyListMemberTypeTests.cpp
        MyListModificationTests.cpp
        MyListRelationalOperatorsTests.cpp)


target_link_libraries(my_list_tests
        gtest_main
        gmock
)

add_test(NAME MyListTests COMMAND my_list_tests)