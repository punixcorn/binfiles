cmake_minimum_required(VERSION 3.5)
project(gitalias)
add_executable(gitalias gitalias.cxx)

# finally made fmt static
set_target_properties(gitalias PROPERTIES CXX_STANDARD 26)

# options
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_library(boost_system boost_execption boost_program_options REQUIRED)

target_link_libraries(
  gitalias PRIVATE boost_program_options boost_system boost_exception -static)


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

