add_executable(tests tests.cpp)

target_link_libraries(tests gtest_main template_storage_lib)

add_test(NAME ExpandTemplateTests COMMAND tests)