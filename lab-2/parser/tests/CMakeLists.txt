add_executable(parser_tests tests.cpp)

target_link_libraries(parser_tests gtest_main url_parser_lib)

add_test(NAME UrlParserTests COMMAND parser_tests)