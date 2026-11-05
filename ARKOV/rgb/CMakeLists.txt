cmake_minimum_required(VERSION 3.16)
project(ARKOV_rgb LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# 1. Fetch github repos
FetchContent_Declare(
    TCS34725
    GIT_REPOSITORY https://github.com/mkyas/cpsy-tcs34725.git
    GIT_TAG main
)

FetchContent_GetProperties(TCS34725)
if(NOT TCS34725_POPULATED)
    FetchContent_MakeAvailable(TCS34725)
endif()

# 2. Set target executable 
add_executable(rgb 
    rgb.cpp
)

# 3. Include headers
target_include_directories(rgb PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${TCS34725_SOURCE_DIR}/interface
)
