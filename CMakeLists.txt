cmake_minimum_required(VERSION 3.16)
project(handheld_desktop LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(handheld-core src/core/canvas.cpp)
target_include_directories(handheld-core PUBLIC src)
target_compile_options(handheld-core PRIVATE -Wall -Wextra -Wpedantic)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

add_executable(handheld-desktop desktop/main.cpp)
target_link_libraries(handheld-desktop PRIVATE handheld-core PkgConfig::SDL2)
target_compile_options(handheld-desktop PRIVATE -Wall -Wextra -Wpedantic)

include(CTest)
if(BUILD_TESTING)
    add_executable(canvas-test tests/canvas_test.cpp)
    target_link_libraries(canvas-test PRIVATE handheld-core)
    add_test(NAME canvas COMMAND canvas-test)
endif()
