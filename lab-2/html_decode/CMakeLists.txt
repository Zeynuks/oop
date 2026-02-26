project(HtmlDecode)

enable_testing()

add_library(html_decode_lib html_decode.cpp html_decode.h)

add_executable(html_decode main.cpp)
target_link_libraries(html_decode html_decode_lib)

add_subdirectory(tests)
