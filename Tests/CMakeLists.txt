find_package(GTest CONFIG REQUIRED)

add_executable(ECS-Tests Tests.cpp)

target_link_libraries(ECS-Tests PRIVATE ${ECS_LIBRARY} GTest::gtest GTest::gtest_main)
#target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_include_directories(ECS-Tests PRIVATE "${CMAKE_SOURCE_DIR}/Source/")

include(GoogleTest)
gtest_discover_tests(ECS-Tests)