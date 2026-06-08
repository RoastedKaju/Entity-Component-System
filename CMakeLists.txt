cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

# Put all executables in one place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Bin)

project(ECS)

file(GLOB_RECURSE SOURCE_FILES "Source/*.cpp")
file(GLOB_RECURSE HEADER_FILES "Source/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Set C++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Link SDL3
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
set(SDL_SHARED ON CACHE BOOL "" FORCE)   # build DLL
set(SDL_STATIC OFF CACHE BOOL "" FORCE)  # skip static lib
add_subdirectory(Vendor/SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Link SDL3 Image
set(SDLIMAGE_VENDORED OFF CACHE BOOL "" FORCE) # use system libs instead of vendored
add_subdirectory(Vendor/SDL3Image)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)

# Link ImGUI
add_subdirectory(Vendor/ImGui)
target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)

# Options
option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

# Warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    if(WARNINGS_AS_ERRORS)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Paths
target_compile_definitions(${PROJECT_NAME} PRIVATE ASSET_DIR="${CMAKE_SOURCE_DIR}/Assets/")