cmake_minimum_required(VERSION 3.14)
project(ARKOV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# 1. Fetch and compile serial without Catkin
FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/wjwwood/serial.git
    GIT_TAG main
)
FetchContent_GetProperties(serial)
if(NOT serial_POPULATED)
    FetchContent_Populate(serial)

    # Build Linux/Unix implementation directly
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

# 2. Find OpenCV for camera processing
find_package(OpenCV REQUIRED)

# 3. Source files
set(SOURCES
    main.cpp
    communication/tests/microControllerCom.cpp
    pid/pidController.cpp
    slam/slamManager.cpp
    voxelVisulization/voxel_map.cpp
    utility/lidar/lidar.cpp
    utility/cameras/cameras.cpp
    localization/particleFilter.cpp
    encoded/driveOdometry.cpp
)

# 4. Target executable
add_executable(arkov_rover ${SOURCES})

# 5. Include header paths
target_include_directories(arkov_rover PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/types
    ${CMAKE_CURRENT_SOURCE_DIR}/communication/tests
    ${CMAKE_CURRENT_SOURCE_DIR}/pid
    ${CMAKE_CURRENT_SOURCE_DIR}/slam
    ${CMAKE_CURRENT_SOURCE_DIR}/voxelVisulization
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/lidar
    ${CMAKE_CURRENT_SOURCE_DIR}/utility/cameras
    ${CMAKE_CURRENT_SOURCE_DIR}/localization
    ${CMAKE_CURRENT_SOURCE_DIR}/encoded
    ${unilidar_sdk2_SOURCE_DIR}/unitree_lidar_sdk/include
    ${OpenCV_INCLUDE_DIRS}
)

# 6. Link dependencies
target_link_libraries(arkov_rover PRIVATE
    serial
    ${OpenCV_LIBS}
    ${unilidar_sdk2_SOURCE_DIR}/unitree_lidar_sdk/lib/aarch64/libunilidar_sdk2.a
)