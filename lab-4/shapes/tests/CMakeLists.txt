add_executable(shapes_tests
        CircleTests.cpp
        RectangleTests.cpp
        TriangleTests.cpp
        LineTests.cpp)

target_link_libraries(shapes_tests
        gtest_main
        gmock
)

add_test(NAME AllShapesTests COMMAND shapes_tests)