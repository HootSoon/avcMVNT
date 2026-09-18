#include "lidar.hh"
#include "types.hh"

lidar::lidar(std::string port){
    ld = unilidar_sdk2::createUnitreeLidarReader();
    p = port;
}

void lidar::initalize(){
    ld->initializeSerial(p); 
    ld->setLidarWorkMode(8); 
    ld->startLidarRotation();
    ld->runParse();
    get_pointcloud();
}

unilidar_sdk2::PointCloudUnitree lidar::get_pointcloud(){
    ld->runParse();
    ld->getPointCloud(cloud);
    return(cloud);
}

std::vector<LiDARPoint> lidar::getLatestScan() {
    unilidar_sdk2::PointCloudUnitree rawCloud;
    ld->getPointCloud(rawCloud);

    std::vector<LiDARPoint> scan;
    scan.reserve(rawCloud.points.size());

    for (const auto& raw_pt : rawCloud.points) {
        LiDARPoint pt;
        pt.x = raw_pt.x;
        pt.y = raw_pt.y;
        pt.z = raw_pt.z;
        pt.intensity = raw_pt.intensity;
        scan.push_back(pt);
    }
    return scan;
}

void lidar::cleanup(){
    
}

