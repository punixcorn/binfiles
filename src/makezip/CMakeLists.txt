
cmake_minimum_required(VERSION 3.5)
project(makezip)
add_executable(${PROJECT_NAME} main.cpp)

# finally made fmt static
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 26)
find_package(fmt)

# options
set(CMAKE_CXX_STANDARD_REQUIRED true)

target_link_libraries(
    ${PROJECT_NAME} PRIVATE  fmt::fmt-header-only zip)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
