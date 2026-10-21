cmake_minimum_required (VERSION 3.6)

SET(SAMPLE_SOURCES
    sampleOnnxMNIST.cpp
)

set(SAMPLE_PARSERS "onnx")

include(../CMakeSamplesTemplate.txt)
include(../set_ifndev.cmake)
