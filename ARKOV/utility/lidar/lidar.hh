#ifndef LIDAR_HH
#define LIDAR_HH

#include "unitree_lidar_sdk.h"

class LiDARPoint;

class lidar{
public:

explicit lidar(std::string port);
~lidar() {cleanup();}

unilidar_sdk2::PointCloudUnitree get_pointcloud();

std::vector<LiDARPoint> getLatestScan();

void initalize();

void cleanup();


private:
std::string p; 
unilidar_sdk2::UnitreeLidarReader *ld;
unilidar_sdk2::PointCloudUnitree cloud;

};


#endif