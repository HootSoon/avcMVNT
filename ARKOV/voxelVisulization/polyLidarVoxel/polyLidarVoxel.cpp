#include "polyscope/polyscope.h"
#include "polyscope/point_cloud.h"
#include "unitree_lidar_sdk.h"
#include "../voxel_map.hh"

#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iostream>

// 100x100x30 at 0.10m resolution = 10m x 10m x 3m physical room volume
VoxelGrid map(100, 100, 30, 0.05);

unilidar_sdk2::UnitreeLidarReader* ld = unilidar_sdk2::createUnitreeLidarReader();
std::atomic<bool> isRunning{true};
std::thread lidarThread;
std::mutex dataMutex;

std::vector<std::array<double, 3>> latestVoxelPoints;
std::vector<double> latestVoxelHeights;
std::atomic<bool> hasNewData{false};

float minDistanceFilter = 0.20f;
// Calculate max physical bounds of the grid from origin (0,0,0)
const float maxBoundX = (map.sizeX / 2.0f) * map.resolution;
const float maxBoundY = (map.sizeY / 2.0f) * map.resolution;
const float maxBoundZ = (map.sizeZ / 2.0f) * map.resolution;



void lidarWorker() {
    unilidar_sdk2::PointCloudUnitree rawCloud;

    while (isRunning) {
        ld->runParse();

        rawCloud.points.clear();
        ld->getPointCloud(rawCloud);

        if (!rawCloud.points.empty()) {
            // Step by 2 (or 3) to skip redundant adjacent rays hitting the same voxel
            for (size_t i = 0; i < rawCloud.points.size(); i += 2) {
                const auto& pt = rawCloud.points[i];
                if (std::isnan(pt.x)) continue;

                float distSq = pt.x * pt.x + pt.y * pt.y + pt.z * pt.z;
                if (distSq <= (minDistanceFilter * minDistanceFilter)) continue;

                // Clamp ray endpoint to the bounding box of the VoxelGrid so 
                // bresenham3D doesn't waste CPU stepping meters outside the grid
                float tx = pt.x, ty = pt.y, tz = pt.z;
                float scale = 1.0f;
                if (std::abs(tx) > maxBoundX) scale = std::min(scale, maxBoundX / std::abs(tx));
                if (std::abs(ty) > maxBoundY) scale = std::min(scale, maxBoundY / std::abs(ty));
                if (std::abs(tz) > maxBoundZ) scale = std::min(scale, maxBoundZ / std::abs(tz));

                map.bresenham3D(0, 0, 0, tx * scale, ty * scale, tz * scale);
            }

            // Build the occupied point list on the worker thread so the GUI never stalls
            std::vector<std::array<double, 3>> tempPoints;
            std::vector<double> tempHeights;

            for (int x = 0; x < map.sizeX; ++x) {
                double wx = (x - map.sizeX / 2.0) * map.resolution;
                for (int y = 0; y < map.sizeY; ++y) {
                    double wy = (y - map.sizeY / 2.0) * map.resolution;
                    for (int z = 0; z < map.sizeZ; ++z) {
                        if (map.getVoxel(x, y, z) > 0) {
                            double wz = (z - map.sizeZ / 2.0) * map.resolution;
                            tempPoints.push_back({wx, wy, wz});
                            tempHeights.push_back(wz);
                        }
                    }
                }
            }

            // Instant pointer swap with the render thread
            {
                std::lock_guard<std::mutex> lock(dataMutex);
                latestVoxelPoints = std::move(tempPoints);
                latestVoxelHeights = std::move(tempHeights);
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
    ld->setLidarWorkMode(8);
    ld->startLidarRotation();

    std::cout << "3. Launching dedicated I/O & Voxel thread..." << std::endl;
    lidarThread = std::thread(lidarWorker);

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
    if (!hasNewData) return;

    std::vector<std::array<double, 3>> renderPoints;
    std::vector<double> renderHeights;

    {
        std::lock_guard<std::mutex> lock(dataMutex);
        renderPoints = std::move(latestVoxelPoints);
        renderHeights = std::move(latestVoxelHeights);
        hasNewData = false;
    }

    if (!renderPoints.empty()) {
        auto* pc = polyscope::registerPointCloud("Voxel Grid Map", renderPoints);
        // Quad render mode is 10x faster on integrated GPUs than 3D Raycast Spheres
        pc->setPointRenderMode(polyscope::PointRenderMode::Quad);
        pc->setPointRadius(map.resolution * 0.45, false);

        auto* scalarQ = pc->addScalarQuantity("Elevation (Z)", renderHeights);
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