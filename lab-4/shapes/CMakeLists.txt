project(Shapes)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

enable_testing()

add_executable(Shapes main.cpp)

target_link_libraries(Shapes sfml-graphics sfml-window sfml-system)

add_subdirectory(tests)
