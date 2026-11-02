cmake_minimum_required(VERSION 3.16)
project(ARKOV_Tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# 1. Fetch serial under 'roarm_serial' using 'main' to intercept roarm_cpp_lite's Catkin call
FetchContent_Declare(
    roarm_serial
    GIT_REPOSITORY https://github.com/wjwwood/serial.git
    GIT_TAG main
)
FetchContent_GetProperties(roarm_serial)
if(NOT roarm_serial_POPULATED)
    FetchContent_Populate(roarm_serial)

    # Build standalone Linux serial backend directly without invoking Catkin
    add_library(serial STATIC
        ${roarm_serial_SOURCE_DIR}/src/serial.cc
        ${roarm_serial_SOURCE_DIR}/src/impl/unix.cc
        ${roarm_serial_SOURCE_DIR}/src/impl/list_ports/list_ports_linux.cc
    )

    target_include_directories(serial PUBLIC
        ${roarm_serial_SOURCE_DIR}/include
    )

    target_link_libraries(serial PUBLIC
        pthread
        rt
    )

    # Alias so targets looking for either 'serial' or 'roarm_serial' resolve cleanly
    add_library(roarm_serial ALIAS serial)
    
    # Expose serial properties under both names to prevent double-fetching
    set(serial_POPULATED TRUE)
    set(serial_SOURCE_DIR ${roarm_serial_SOURCE_DIR})
endif()

# 2. Fetch RoArm C++ Lite (inherits the pre-populated roarm_serial target)
find_package(Threads REQUIRED)

if(NOT TARGET roarm_cpp_lite)
    FetchContent_Declare(
        roarm_cpp_lite
        GIT_REPOSITORY https://github.com/HootSoon/roarm_cpp_lite.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(roarm_cpp_lite)
endif()

# 3. Unitree LiDAR SDK
FetchContent_Declare(
    unilidar_sdk2
    GIT_REPOSITORY https://github.com/unitreerobotics/unilidar_sdk2.git
    GIT_TAG main
)
FetchContent_GetProperties(unilidar_sdk2)
if(NOT unilidar_sdk2_POPULATED)
    FetchContent_Populate(unilidar_sdk2)
endif()

# 4. OpenCV
find_package(OpenCV REQUIRED)

# 5. Shared Core Sources (paths relative to ARKOV/Tests/)
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

# 6. Shared Static Library
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
    ${OpenCV_INCLUDE_DIRS}
)

# Architecture check for Unitree static binary
if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
    set(UNILIDAR_ARCH "aarch64")
else()
    set(UNILIDAR_ARCH "x86_64")
endif()

target_link_libraries(arkov_core PUBLIC
    serial
    roarm_cpp_lite
    Threads::Threads
    ${OpenCV_LIBS}
    ${unilidar_sdk2_SOURCE_DIR}/unitree_lidar_sdk/lib/${UNILIDAR_ARCH}/libunilidar_sdk2.a
)

# 7. Test Targets (drive_controller_arm and pid_controller_vel)
set(TEST_SOURCES
    drive_controller_arm.cpp
    pid_controller_vel.cpp
)

foreach(test_file ${TEST_SOURCES})
    get_filename_component(test_target ${test_file} NAME_WE)
    add_executable(${test_target} ${test_file})
    target_link_libraries(${test_target} PRIVATE arkov_core)
endforeach()