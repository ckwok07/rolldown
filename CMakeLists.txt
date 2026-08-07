cmake_minimum_required(VERSION 3.20)
set(CMAKE_TOOLCHAIN_FILE "/Users/christiankwok/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
project(rolldown)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(raylib CONFIG REQUIRED)


add_executable(rolldown
    src/main.cpp
    src/Engine.cpp
    src/Globals.cpp
    src/gui/App.cpp
    src/gui/Drag.cpp
)

target_include_directories(rolldown PRIVATE
    src
    /opt/homebrew/include
)

target_link_libraries(rolldown PRIVATE
    imgui::imgui
    raylib
)