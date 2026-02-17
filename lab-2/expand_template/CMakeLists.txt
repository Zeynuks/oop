project(ExpandTemplate)

enable_testing()

add_library(expand_template_lib
        ExpandTemplate.h
        Parsers.h
        TemplateStorage.cpp
        TemplateStorage.h
)

add_executable(ExpandTemplate main.cpp)
target_link_libraries(ExpandTemplate expand_template_lib)

add_subdirectory(tests)