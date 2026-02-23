project(Calculator)

enable_testing()

add_library(menu_lib
        Menu.cpp
        Menu.hpp
)

add_subdirectory(core)

add_executable(Calculator main.cpp)
target_link_libraries(Calculator menu_lib)
target_link_libraries(Calculator calculator_lib)

add_subdirectory(tests)