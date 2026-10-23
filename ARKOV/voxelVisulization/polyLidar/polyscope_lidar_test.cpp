#include "polyscope/polyscope.h"
#include "polyscope/point_cloud.h"
#include "unitree_lidar_sdk.h"

#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>

unilidar_sdk2::UnitreeLidarReader* ld = unilidar_sdk2::createUnitreeLidarReader();
std::atomic<bool> isRunning{true};
std::thread lidarThread;
std::mutex dataMutex;

std::vector<std::array<float, 3>> latestScanBatch;
bool hasNewData = false;

size_t maxAccumulatedPoints = 60000;
float minDistanceFilter = 0.20f; 

std::vector<std::array<float, 3>> accumulatedPoints;
std::vector<float> pointHeights;


// Lidar worker to process lidar packets on a background thread
void lidarWorker() {
    unilidar_sdk2::PointCloudUnitree rawCloud;

    while (isRunning) {
        ld->runParse();

        rawCloud.points.clear();
        ld->getPointCloud(rawCloud);

        if (!rawCloud.points.empty()) {
            std::vector<std::array<float, 3>> validPoints;
            validPoints.reserve(rawCloud.points.size());

            for (const auto& pt : rawCloud.points) {
                // Drop returns within near blindspot (< 20cm)
                float distSq = pt.x * pt.x + pt.y * pt.y + pt.z * pt.z;
                if (distSq > (minDistanceFilter * minDistanceFilter) && !std::isnan(pt.x)) {
                    validPoints.push_back({pt.x, pt.y, pt.z});
                }
            }

            if (!validPoints.empty()) {
                std::lock_guard<std::mutex> lock(dataMutex);
                latestScanBatch = std::move(validPoints);
                hasNewData = true;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(500));
        }
    }
}



void initializeLidar() {
    std::cout << "--- HARDWARE DIAGNOSTIC START ---" << std::endl;
    std::string port = "/dev/ttyACM0";

    std::cout << "1. Opening port " << port << std::endl;
    int success = ld->initializeSerial(port);
    if (success != 0) {
        std::cerr << "CRITICAL ERROR: Could not connect to LiDAR on " << port << "!" << std::endl;
        exit(1);
    }

    std::cout << "2. Configuring LiDAR to Serial Mode (workMode = 8)..." << std::endl;
    ld->setLidarWorkMode(8); // Mode 8: serial
    ld->startLidarRotation();

    std::cout << "3. Launching dedicated I/O thread..." << std::endl;
    lidarThread = std::thread(lidarWorker);

    // Checks if joinable, keeps checking until true which means side thread has started, then it can continue.
    int attempts = 0;
    while (!hasNewData) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        if (++attempts > 60) {
            std::cerr << "!!! ERROR: Timeout waiting for initial LiDAR packets. !!!" << std::endl;
            isRunning = false;
            if (lidarThread.joinable()) lidarThread.join();
            exit(1);
        }
    }
    std::cout << ">>> SUCCESS: LiDAR stream active!" << std::endl;
}



void renderCallback() {
    bool updated = false;

    {
        std::lock_guard<std::mutex> lock(dataMutex);
        if (hasNewData) {
            accumulatedPoints.insert(accumulatedPoints.end(), 
                                     latestScanBatch.begin(), 
                                     latestScanBatch.end());
            hasNewData = false;
            updated = true;
        }
    }

    if (updated) {
        // Trim rolling buffer
        if (accumulatedPoints.size() > maxAccumulatedPoints) {
            size_t excess = accumulatedPoints.size() - maxAccumulatedPoints;
            accumulatedPoints.erase(accumulatedPoints.begin(), accumulatedPoints.begin() + excess);
        }

        pointHeights.resize(accumulatedPoints.size());
        for (size_t i = 0; i < accumulatedPoints.size(); ++i) {
            pointHeights[i] = accumulatedPoints[i][2];
        }

        auto* pc = polyscope::registerPointCloud("Accumulated LiDAR Scan", accumulatedPoints);
        pc->setPointRadius(0.006);
        pc->setPointRenderMode(polyscope::PointRenderMode::Sphere);

        auto* scalarQ = pc->addScalarQuantity("Elevation (Z)", pointHeights);
        scalarQ->setColorMap("turbo");
        scalarQ->setEnabled(true);
    }
}



int main() {
    initializeLidar();

    polyscope::init();
    polyscope::state::userCallback = renderCallback;
    polyscope::show();

    isRunning = false;
    if (lidarThread.joinable()) {
        lidarThread.join();
    }

    return 0;
}