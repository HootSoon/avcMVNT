cmake_minimum_required(VERSION 3.18)
project(roarm_ik_test LANGUAGES CXX)

include(FetchContent)
find_package(Threads REQUIRED)

if(NOT TARGET roarm_cpp_lite)
    FetchContent_Declare(roarm_cpp_lite
        GIT_REPOSITORY https://github.com/HootSoon/roarm_cpp_lite.git
        GIT_TAG main
    )
    FetchContent_MakeAvailable(roarm_cpp_lite)
endif()

add_executable(roarm_ik_test
    test.cpp
    roarm_ik.cpp
)

target_compile_features(roarm_ik_test PRIVATE cxx_std_17)

target_link_libraries(roarm_ik_test PRIVATE
    roarm_cpp_lite
    Threads::Threads
)