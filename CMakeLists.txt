cmake_minimum_required(VERSION 4.3)
project(zutomayo_listening)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
add_executable(zutomayo_listening src/main.cpp
        src/models/Album.hpp)
target_link_libraries(zutomayo_listening PRIVATE sfml-graphics sfml-window sfml-system)
target_compile_options(zutomayo_listening PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
target_include_directories(zutomayo_listening PRIVATE src)