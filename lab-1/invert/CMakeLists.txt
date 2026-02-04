project(invert)

add_library(matrix_lib INTERFACE)

target_include_directories(matrix_lib INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/MatrixLib
)

add_executable(invert invert.cpp)
target_link_libraries(invert PRIVATE matrix_lib)