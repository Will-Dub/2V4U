cmake_minimum_required(VERSION 3.20)
project(vcu_tests CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
file(GLOB_RECURSE LOGIC_SRC CONFIGURE_DEPENDS ${ROOT}/App/Src/Logic/*.cpp)
file(GLOB_RECURSE TEST_SRC CONFIGURE_DEPENDS Unit/*.cpp Integration/*.cpp)

add_executable(vcu_tests
    ${LOGIC_SRC}
    Unit/test_VcuStateStandby.cpp)

target_include_directories(vcu_tests PRIVATE
    ${ROOT}/App/Inc/Logic
    ${CMAKE_SOURCE_DIR}/Fakes)

target_link_libraries(vcu_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(vcu_tests)