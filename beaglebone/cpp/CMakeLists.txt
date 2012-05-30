cmake_minimum_required(VERSION 2.8)
set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library)
add_library (BeagleLib ${LIBRARY_DIR}/gpio.cpp) 
include_directories(${LIBRARY_DIR})
