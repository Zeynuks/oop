project(EconomyEmulation)

enable_testing()

add_subdirectory(core)
add_subdirectory(actors)

add_library(emulation_lib ActorBuilder.cpp EconomyEmulator.cpp)

add_executable(EconomyEmulation main.cpp)

target_link_libraries(EconomyEmulation
        PRIVATE
        emulation_lib
        economy_core_lib
        economy_actors_lib
)

add_subdirectory(tests)