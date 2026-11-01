cmake_minimum_required(VERSION 3.14)
project(ARKOV_Tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# 1. Fetch dependencies
FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/wjwwood/serial.git
    GIT_TAG master
)
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

    target_link_libraries(serial PUBLIC
        pthread
        rt
    )
endif()

FetchContent_Declare(
    unilidar_sdk2
    GIT_REPOSITORY https://github.com/unitreerobotics/unilidar_sdk2.git
    GIT_TAG main
)
FetchContent_GetProperties(unilidar_sdk2)
if(NOT unilidar_sdk2_POPULATED)
    FetchContent_Populate(unilidar_sdk2)
endif()

find_package(OpenCV REQUIRED)

# 2. Shared core rover sources (excluding test mains)
set(SHARED_SOURCES
    ../communication/tests/microControllerCom.cpp
    ../pid/pidController.cpp
    ../slam/slamManager.cpp
    ../voxelVisulization/voxel_map.cpp
    ../utility/lidar/lidar.cpp
    ../utility/cameras/cameras.cpp
    ../localization/particleFilter.cpp
    ../encoded/driveOdometry.cpp
    ../utility/controller/controller.cpp
    ../manipulation/roarm_ik.cpp
)

# 3. Create a common static library to share across tests
add_library(arkov_core STATIC ${SHARED_SOURCES})

target_include_directories(arkov_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../types
    ${CMAKE_CURRENT_SOURCE_DIR}/../communication/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/../pid
    ${CMAKE_CURRENT_SOURCE_DIR}/../slam
    ${CMAKE_CURRENT_SOURCE_DIR}/../voxelVisulization
    ${CMAKE_CURRENT_SOURCE_DIR}/../utility/lidar
    ${CMAKE_CURRENT_SOURCE_DIR}/../utility/cameras
    ${CMAKE_CURRENT_SOURCE_DIR}/../localization
    ${CMAKE_CURRENT_SOURCE_DIR}/../encoded
    ${CMAKE_CURRENT_SOURCE_DIR}/../utility/controller
    ${CMAKE_CURRENT_SOURCE_DIR}/../manipulation
    ${unilidar_sdk2_SOURCE_DIR}/unitree_lidar_sdk/include
)

# Architecture check for Unitree static binary
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(UNILIDAR_ARCH "aarch64")
else()
    set(UNILIDAR_ARCH "x86_64")
endif()

target_link_libraries(arkov_core PUBLIC
    serial
    ${OpenCV_LIBS}
    ${unilidar_sdk2_SOURCE_DIR}/unitree_lidar_sdk/lib/${UNILIDAR_ARCH}/libunilidar_sdk2.a
)

# 4. Define test files to build
set(TEST_SOURCES
    drive_controller_arm.cpp
    pid_controller_vel.cpp
)

# Alternatively, automatically collect all .cpp files directly inside Tests/:
# file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 5. Build an executable for each test file
foreach(test_file ${TEST_SOURCES})
    # Extracts the file name without directory or extension (e.g., "drive_controller_arm")
    get_filename_component(test_target ${test_file} NAME_WE)

    add_executable(${test_target} ${test_file})
    target_link_libraries(${test_target} PRIVATE arkov_core)
endforeach()