cmake_minimum_required(VERSION 3.20)
project(rolldown)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)


add_executable(rolldown
    src/main.cpp
    src/Engine.cpp
    src/Globals.cpp
    src/gui/Gui.cpp
)

target_include_directories(rolldown PRIVATE
    src
    C:/Users/Uffuf/vcpkg/installed/x64-windows/include
)

target_link_libraries(rolldown PRIVATE
    glfw
    OpenGL::GL
    imgui::imgui
)