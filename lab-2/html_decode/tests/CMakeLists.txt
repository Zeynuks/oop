add_executable(html_decode_tests tests.cpp)

target_link_libraries(html_decode_tests gtest_main html_decode_lib)

add_test(NAME HtmlDecodeTests COMMAND tests)
