set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.16)

# 1. Locate system packages
find_package(OpenCV REQUIRED)

# 2. Build executable
add_executable(detections 
    detections.cpp
)

# 3. Include headers
target_include_directories(detections PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

# 4. Link libraries
target_link_libraries(detections PRIVATE 
    ${OpenCV_LIBS}
)
