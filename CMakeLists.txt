cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/Vendor/VCPKG/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Put all executables in one place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Bin)

project(ECS-Executable)
set(ECS_LIBRARY ECS-Library)

set(ECS_SOURCES 
    "Source/Actor.cpp" 
    "Source/Application.cpp" 
    "Source/Systems.cpp" 
    "Source/Texture.cpp" 
    "Source/UI.cpp" 
    "Source/World.cpp" 
)

set(ECS_HEADERS 
    "Source/Actor.hpp" 
    "Source/Application.hpp" 
    "Source/Common.hpp" 
    "Source/Components.hpp" 
    "Source/Entity.hpp" 
    "Source/Systems.hpp" 
    "Source/Texture.hpp" 
    "Source/UI.hpp" 
    "Source/World.hpp" 
)

add_library(${ECS_LIBRARY})
target_sources(${ECS_LIBRARY} PRIVATE ${ECS_SOURCES} ${ECS_HEADERS})

# Set C++ standard
target_compile_features(${ECS_LIBRARY} PRIVATE cxx_std_20)

# Link SDL3
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
target_link_libraries(${ECS_LIBRARY} PUBLIC SDL3::SDL3 SDL3_image::SDL3_image)

# Link ImGUI
add_subdirectory(Vendor/ImGui)
target_link_libraries(${ECS_LIBRARY} PUBLIC ImGui)

# Options
option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# Warnings
if(MSVC)
    target_compile_options(${ECS_LIBRARY} PRIVATE /W4)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(${ECS_LIBRARY} PRIVATE /WX)
    endif()
else()
    target_compile_options(${ECS_LIBRARY} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Paths
target_compile_definitions(${ECS_LIBRARY} PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/Assets/")

# Executable wrapper
add_executable(${PROJECT_NAME} "Source/Main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE ${ECS_LIBRARY})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/Source")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
