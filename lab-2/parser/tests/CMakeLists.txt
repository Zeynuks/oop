add_executable(tests tests.cpp)

target_link_libraries(tests gtest_main url_parser_lib)

add_test(NAME UrlParserTests COMMAND tests)