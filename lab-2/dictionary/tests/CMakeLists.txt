add_executable(tests
        LoaderTest.cpp
        DictionaryTest.cpp
        InMemoryStorageTest.cpp
)

target_link_libraries(tests
        GTest::gtest
        GTest::gtest_main
        GTest::gmock
        dictionary_lib
        storage_lib
)

add_test(NAME DictionaryTests COMMAND tests)
