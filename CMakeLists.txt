cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(ECS)

file(GLOB_RECURSE SOURCE_FILES "Source/*.cpp")
file(GLOB_RECURSE HEADER_FILES "Source/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Set C++ standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Link SDL3
add_subdirectory(Vendor/SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Link SDL3 Image
set(SDLIMAGE_VENDORED OFF CACHE BOOL "" FORCE) # use system libs instead of vendored
add_subdirectory(Vendor/SDL3Image)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3_image::SDL3_image)

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

# TODO: Add post build command to copy over SDL DLLs