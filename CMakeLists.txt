cmake_minimum_required(VERSION 3.21)
project(PhysicsSimulator LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(PhysicsSimulator src/main.cpp)
target_link_libraries(PhysicsSimulator PRIVATE sfml-graphics)
target_compile_features(PhysicsSimulator PRIVATE cxx_std_17)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET PhysicsSimulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:PhysicsSimulator> $<TARGET_FILE_DIR:PhysicsSimulator> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS PhysicsSimulator)
