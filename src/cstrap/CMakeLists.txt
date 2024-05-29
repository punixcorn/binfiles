cmake_minimum_required(VERSION 3.5)
project(cstrap)
add_executable(${PROJECT_NAME} cstrap.cxx)

# finally made fmt static
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 26)

# options
set(CMAKE_CXX_STANDARD_REQUIRED true)
target_link_libraries( ${PROJECT_NAME} PUBLIC "-static")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

