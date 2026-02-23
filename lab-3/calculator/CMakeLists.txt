project(Calculator)

enable_testing()

add_library(menu_lib
        Menu.cpp
        Menu.hpp
)

add_executable(Calculator main.cpp)
target_link_libraries(Calculator menu_lib)

add_subdirectory(core)
add_subdirectory(tests)