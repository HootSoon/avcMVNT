#ifndef SLAM_MANAGER_HH
#define SLAM_MANAGER_HH

#include <vector>

// Forward declarations with exact class names
class particleFilter;
class driveOdometry;
class VoxelGrid;
class lidar;

class slamManager {
public:
    slamManager(particleFilter* p_filter, driveOdometry* d_odom, VoxelGrid* v_map, lidar* l_sensor);
    void startUp();
    void update();

    float curx;
    float cury;
    float heading;

private:
    particleFilter* pf;
    driveOdometry* driveOdom;
    VoxelGrid* map;
    lidar* ld;
};

#endif