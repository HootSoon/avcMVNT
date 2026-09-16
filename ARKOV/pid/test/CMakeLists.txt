cmake_minimum_required(VERSION 3.14)
project(ARKOV LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 3. Add executable
add_executable(pidTest 
    pidTest.cpp 
    pidController.cpp
)

# 4. Include the exact folder where unitree_lidar_sdk.h lives
target_include_directories(pidTest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}../.
    ${CMAKE_CURRENT_SOURCE_DIR}/../../types
)

# 5. Link the exact aarch64 library for your Jetson AGX
target_link_libraries(pidTest PRIVATE 
)