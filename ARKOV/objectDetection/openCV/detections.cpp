#include <iostream>
#include <chrono>
#include <onnxruntime_cxx_api.h>


#include "cameras.hh"
#include "yolos/yolos.hpp"

using namespace yolos::det;

int main(){    
    std::string modelPath = "/home/uafs/Documents/YOLOs-CPP/models/yolo26n-depth.onnx";
    std::string labelsPath = "/home/uafs/Documents/YOLOs-CPP/models/imagenet.names";
    bool useGPU = true;

    YOLODetector detector(modelPath, labelsPath, useGPU);

    cv_Webcam cam(0);
    cv::Mat frame;

    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    while(true){
        cam.update_frame(frame);
        frameCount++;

        // Inference
        auto detectStart = std::chrono::high_resolution_clock::now();
        std::vector<Detection> detections = detector.detect(frame);
        auto detectEnd = std::chrono::high_resolution_clock::now();
        auto detectDuration = std::chrono::duration_cast<std::chrono::milliseconds>(detectEnd - detectStart);

        detector.drawDetections(frame, detections);

        // Depth Estimation
        // frame estimate(const frame);

        // Info Overlay
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
        double fps = elapsed.count() > 0 ? frameCount / static_cast<double>(elapsed.count()) : 0;

        std::string info = "FPS: " + std::to_string(static_cast<int>(fps)) +
                           "Objects: " + std::to_string(detections.size()) +
                           "Time: " + std::to_string(detectDuration.count());

        cv::putText(frame, info, cv::Point(10,30), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.7, 
                    cv::Scalar(0, 255, 0), 2);
        cam.display_frame(frame);

        if (cv::waitKey(1) == 27 || cv::waitKey(1) == 'q') {
            break;
        }
    }

    cam.cleanup();
    return 0;

}
