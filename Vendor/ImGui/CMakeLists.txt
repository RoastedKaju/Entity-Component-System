cmake_minimum_required(VERSION 3.24)
project(ImGui)

add_library(ImGui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    backends/imgui_impl_sdl3.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(ImGui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

target_link_libraries(ImGui PUBLIC SDL3::SDL3)
