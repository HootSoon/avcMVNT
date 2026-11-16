cmake_minimum_required(VERSION 3.16)

project(CVDNN LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# If OpenCV is not found automatically, uncomment and set this:
# set(OpenCV_DIR "/usr/local/lib/cmake/opencv4")

find_package(OpenCV REQUIRED)

add_executable(CVDNN
    src/CVDNN.cpp
)

target_include_directories(CVDNN PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(CVDNN PRIVATE
    ${OpenCV_LIBS}
)

# Optional: useful for debugging
message(STATUS "OpenCV version: ${OpenCV_VERSION}")