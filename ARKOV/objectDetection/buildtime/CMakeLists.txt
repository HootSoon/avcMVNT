cmake_minimum_required (VERSION 3.6)

SET(SAMPLE_SOURCES
    sampleOnnxMNIST.cpp
)

set(SAMPLE_PARSERS "onnx")

include(../set_ifndef.cmake)
include(../CMakeSamplesTemplate.txt)
