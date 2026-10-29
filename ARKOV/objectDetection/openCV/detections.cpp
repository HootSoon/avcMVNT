#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>

#include "../../utility/cameras/cameras.hh"

#include "yolos/tasks/detection.hpp"
#include "yolos/tasks/depth.hpp"
#include "utils.hpp"

int main(){
    cv_Webcam cam(0);
    cv::Mat frame;
    
    std::string modelPath = "/home/uafs/Documents/YOLOs-CPP/models/yolo26n-depth.onnx";
    std::string labelsPath = "/home/uafs/path/to/labels.names";

    bool useGPU = true;
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    while(cam.update_frame(frame)){
        frameCount++;

        // Inference
        auto detectStart = std::chrono::high_resolution_clock::now();
        std::vector<Detection> detections = detector.detect(frame);
        auto detectEnd = std::chrone::high_resolution_clock::now();
        auto detectDuration = std::chrono::duration_cast<std::chrono::milliseconds>(detectEnd - detectStart);

        detector.drawDetections(frame, detections);

        // Depth Estimation
        frame estimate(const frame);

        // Info Overlay
        auto currentTime = std::chrono:high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
        double fps = elapsed.count() > 0 ? frameCount / static_cast<double>(elapsed.count()) : 0;

        std::string info = "FPS: " + std::to_string(static_cast<int>(fps)) +
                           "Objects: " + std::to_string(detections.size()) +
                           "Time: " + std::to_string(detectDuration.count());

        cv::putText(frame, info);
        cam.display_frame(frame);

        if (cv::waitKey(1) == 27 || cv::waitKey(1) == 'q') {
            break;
        }
    }

    cam.cleanup();

}
