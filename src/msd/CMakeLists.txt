cmake_minimum_required(VERSION 3.5)
project(msd)
add_executable(${PROJECT_NAME} msd.cxx ../engine/engine.hpp)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 26)

target_link_libraries(${PROJECT_NAME} PUBLIC "-static")
# options
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

