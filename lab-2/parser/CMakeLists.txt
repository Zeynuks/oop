project(url_parser)

enable_testing()

add_library(url_parser_lib parser.cpp parser.h)

add_executable(url_parser main.cpp)
target_link_libraries(url_parser url_parser_lib)

add_subdirectory(tests)