cmake_minimum_required(VERSION 3.14)
project(ARKOV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Fetch dependency for serial communication (microControllerCom)
include(FetchContent)
FetchContent_Declare(
    serial
    GIT_REPOSITORY https://github.com/wjwwood/serial.git
    GIT_TAG master
)
FetchContent_MakeAvailable(serial)

# Find OpenCV for camera processing
find_package(OpenCV REQUIRED)

# All source files matching your includes
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

# Target executable
add_executable(arkov_rover ${SOURCES})

# Include header paths matching your project layout
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
)

# Link required libraries
target_link_libraries(arkov_rover PRIVATE
    serial
    ${OpenCV_LIBS}
)
