# moving on to adding libgit + ftxui = a tui git ( just for fun )
cmake_minimum_required(VERSION 3.25)
project(gitalias)
add_executable(${PROJECT_NAME} Main.cxx)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
find_library(boost_system boost_execption boost_program_options REQUIRED)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE boost_program_options boost_system boost_exception
                          -static) # -static ( build static ) no need for
                                   # makefile anymore
include(GoogleTest)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
