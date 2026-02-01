add_executable(tests tests.cpp)

target_link_libraries(tests gtest_main html_decode_lib)

add_test(NAME HtmlDecodeTests COMMAND tests)
