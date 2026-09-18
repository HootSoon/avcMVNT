#include "slamManager.hh"
#include "particleFilter.hh"
#include "driveOdometry.hh"
#include "voxel_map.hh"
#include "lidar.hh"

#include <vector>
#include <iostream>

#include "types.hh" 

#ifndef SENSOR_Z_OFFSET
#define SENSOR_Z_OFFSET 0.15 // Set physical sensor mounting Z offset in meters
#endif


slamManager::slamManager(particleFilter* p_filter, driveOdometry* d_odom, VoxelGrid* v_map, lidar* l_sensor)
    : pf(p_filter), driveOdom(d_odom), map(v_map), ld(l_sensor){
        curx = 0.0;
        cury = 0.0;
    }

void slamManager::startUp() {
    std::vector<LiDARPoint> scan = ld->getLatestScan();
    Particle bestP = pf->getBestPose();

    for (const auto& pt : scan) {
        map->bresenham3D(bestP.x, bestP.y, SENSOR_Z_OFFSET, pt.world_x, pt.world_y, pt.world_z);
    }
}

void slamManager::update() {
    float dist = 0.0, dtheta = 0.0;
    // I input 0 for encoders for now
    driveOdom->calcDistanceTraveled(0,0);
    heading += dtheta;

    float dx = std::cos((3.14159/180)*heading);
    float dy = std::sin((3.14159/180)*heading);

    pf->predict(dx, dy, dtheta);
    std::vector<LiDARPoint> scan = ld->getLatestScan();
    pf->updateWeights(*map, scan);
    pf->resample();

    Particle bestP = pf->getBestPose();
    for (const auto& pt : scan) {
        map->bresenham3D(bestP.x, bestP.y, SENSOR_Z_OFFSET, pt.world_x, pt.world_y, pt.world_z);
    }
}