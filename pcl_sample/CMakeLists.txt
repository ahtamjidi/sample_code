cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(NDT)
FIND_PACKAGE(PCL 1.5 REQUIRED)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR} )

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(ndt_tutorial ndt_tutorial.cpp)
target_link_libraries(ndt_tutorial ${PCL_LIBRARIES})