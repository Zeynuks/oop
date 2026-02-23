set(CMAKE_CXX_STANDARD 23)

project(Dictionary)

enable_testing()

file(GLOB_RECURSE CORE_SOURCES "core/*.cpp")
add_library(dictionary_lib ${CORE_SOURCES})

file(GLOB_RECURSE STORE_SOURCES "storage/*.cpp")
add_library(storage_lib ${STORE_SOURCES})

add_executable(dictionary main.cpp)

target_link_libraries(dictionary PRIVATE dictionary_lib storage_lib)

add_subdirectory(tests)
