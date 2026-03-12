project(Car)

enable_testing()

add_library(car_lib
        Car.cpp
        Car.hpp
)

add_executable(Car main.cpp)
target_link_libraries(Car car_lib)

add_subdirectory(tests)