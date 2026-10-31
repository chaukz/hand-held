cmake_minimum_required(VERSION 3.16)
project(handheld_desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

add_executable(handheld-desktop
    desktop/main.cpp
    src/core/canvas.cpp
)
target_include_directories(handheld-desktop PRIVATE src)
target_link_libraries(handheld-desktop PRIVATE PkgConfig::SDL2)
target_compile_options(handheld-desktop PRIVATE -Wall -Wextra -Wpedantic)
