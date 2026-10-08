cmake_minimum_required(VERSION 3.14)
project(ARKOV)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/wjwwood/serial.git
    GIT_TAG        main
)

# Populate files without executing serial's ROS-dependent CMakeLists.txt
FetchContent_GetProperties(serial)
if(NOT serial_POPULATED)
    FetchContent_Populate(serial)

    add_library(serial STATIC
        ${serial_SOURCE_DIR}/src/serial.cc
        ${serial_SOURCE_DIR}/src/impl/unix.cc
        ${serial_SOURCE_DIR}/src/impl/list_ports/list_ports_linux.cc
    )

    target_include_directories(serial PUBLIC
        ${serial_SOURCE_DIR}/include
    )

    find_package(Threads REQUIRED)
    target_link_libraries(serial PUBLIC Threads::Threads rt)
endif()

add_executable(motorTest controllerMotorTest.cpp)
target_link_libraries(motorTest PRIVATE serial)