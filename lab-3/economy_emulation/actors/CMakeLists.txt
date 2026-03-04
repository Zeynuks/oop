add_library(economy_actors_lib
        ./Apu.cpp
        ./Apu.hpp
        ./Bart.cpp
        ./Bart.hpp
        ./Berns.cpp
        ./Berns.hpp
        ./Chester.cpp
        ./Chester.hpp
        ./Homer.cpp
        ./Homer.hpp
        ./Liza.cpp
        ./Liza.hpp
        ./Marge.cpp
        ./Marge.hpp
        ./Nelson.cpp
        ./Nelson.hpp
        ./Random.hpp
        ./Weylon.cpp
        ./Weylon.hpp
)

target_include_directories(economy_actors_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(economy_actors_lib PUBLIC economy_core_lib)