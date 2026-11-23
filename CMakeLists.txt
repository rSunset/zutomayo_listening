cmake_minimum_required(VERSION 4.3)
project(zutomayo_listening)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ZUTOMAYO_WARNINGS -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_library(zutomayo_core STATIC src/logic/AlbumGenerator.cpp
        src/logic/Scheduling.cpp
        src/logic/Scheduling.hpp)
target_include_directories(zutomayo_core PUBLIC src)
target_compile_options(zutomayo_core PRIVATE ${ZUTOMAYO_WARNINGS})

add_executable(zutomayo_listening src/main.cpp)
target_link_libraries(zutomayo_listening PRIVATE zutomayo_core sfml-graphics sfml-window sfml-system)
target_compile_options(zutomayo_listening PRIVATE ${ZUTOMAYO_WARNINGS})

enable_testing()
add_executable(core_tests tests/core_tests.cpp)
target_link_libraries(core_tests PRIVATE zutomayo_core)
target_compile_options(core_tests PRIVATE ${ZUTOMAYO_WARNINGS})
add_test(NAME core_tests COMMAND core_tests)