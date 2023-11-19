# finally made fmt static
cmake_minimum_required(VERSION 3.5)
project(makehere)
add_executable(${PROJECT_NAME} Main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED true)
find_package(fmt)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE fmt::fmt-header-only
                          -static)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
