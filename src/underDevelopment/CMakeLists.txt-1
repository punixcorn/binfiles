cmake_minimum_required(VERSION 3.5)
project(binfiles)
add_executable(gitalias gitalias.cxx)
add_executable(makehere makehere.cxx)
add_executable(msd msd.cxx)

# finally made fmt static
set_target_properties(gitalias makehere msd PROPERTIES CXX_STANDARD 23)

# options
set(CMAKE_CXX_STANDARD_REQUIRED true)

find_library(boost_system boost_execption boost_program_options REQUIRED)
find_package(fmt)

target_link_libraries(
  gitalias PRIVATE boost_program_options boost_system boost_exception fmt::fmt-header-only
                          -static)

target_link_libraries(
   makehere PRIVATE fmt::fmt-header-only
                          -static)

target_link_libraries(
   msd PRIVATE fmt::fmt-header-only
                          -static)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# add_subdirectory(makehere)
# add_subdirectory(gitalias)
