cmake_minimum_required(VERSION 2.8)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#Configure GLib (interrupts)
find_package(GLIB_PKG REQUIRED)
include_directories(${GLIB_INCLUDE_DIR} ${GLIB_CONFIG_INCLUDE_DIR})


set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/library)
add_library (BeagleLib ${LIBRARY_DIR}/gpio.cpp ${LIBRARY_DIR}/register.cpp ${LIBRARY_DIR}/eqep.cpp) 
target_link_libraries(BeagleLib pthread)

include_directories(${LIBRARY_DIR})
add_executable(blink tests/blink.cpp)
add_executable(interrupts tests/interrupts.cpp)

target_link_libraries(blink BeagleLib)
target_link_libraries(interrupts BeagleLib)
