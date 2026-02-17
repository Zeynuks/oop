project(ExpandTemplate)

enable_testing()

add_library(template_storage_lib
        TemplateStorage.cpp
        TemplateStorage.h
)

add_executable(ExpandTemplate main.cpp)
target_link_libraries(ExpandTemplate template_storage_lib)

add_subdirectory(tests)