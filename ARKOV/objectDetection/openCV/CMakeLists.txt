set(ONNXRUNTIME_DIR "/home/uafs/Downloads/onnxruntime-linux-aarch64-1.31.1" 
	CACHE PATH "Path to ONNX Runtime installation")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)

include(FetchContent)

# 1. Locate system packages
find_package(OpenCV REQUIRED)
find_package(onnxruntime REQUIRED)

# 2. Fetch github repos
FetchContent_Declare(
    yolos_cpp
    GIT_REPOSITORY https://github.com/Geekgineer/YOLOs-CPP.git
    GIT_TAG main
)

FetchContent_Declare(
    onnxruntime_api
    GIT_REPOSITORY https://github.com/microsoft/onnxruntime.git
    GIT_TAG main
)

FetchContent_GetProperties(yolos_cpp)
if(NOT yolos_cpp_POPULATED)
    FetchContent_MakeAvailable(yolos_cpp)
endif()

FetchContent_GetProperties(onnxruntime_api)
if(NOT onnxruntime_api_POPULATED)
    FetchContent_MakeAvailable(onnxruntime_api)
endif()

# 3. Build executable
add_executable(detections 
    detections.cpp
    ../../utility/cameras/cameras.cpp
)


# 4. Include headers
target_include_directories(detections PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    ${onnxruntime_INCLUDE_DIRS}/onnxruntime/include
    ${onnxruntime_api_SOURCE_DIR}/include/onnxruntime/core/session
    ${yolos_cpp_SOURCE_DIR}/include
    ../../utility/cameras
)

# 5. Link libraries
target_link_libraries(detections PRIVATE 
    ${OpenCV_LIBS}
    ${onnxruntime_LIBRARY}
)
