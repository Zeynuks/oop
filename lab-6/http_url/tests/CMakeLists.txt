add_executable(http_url_tests tests.cpp)

target_link_libraries(http_url_tests
        gtest_main
        gmock
)

add_test(NAME HttpUrlTests COMMAND http_url_tests)