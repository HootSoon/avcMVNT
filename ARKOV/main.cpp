#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "pid/pidController.hh"
#include "slam/slamManager.hh"
#include "voxelVisulization/voxel_map.hh"
#include "utility/lidar/lidar.hh"
#include "utility/cameras/cameras.hh"
#include "localization/particleFilter.hh"
#include "encoded/driveOdometry.h"
#include <unistd.h>

microcontroller stm32("COM13", 115200);
pidController leftPID(0.30, 0.05, 0.10);
pidController rightPID(0.30, 0.05, 0.10);
VoxelGrid map(100, 100, 50, 0.1);          
lidar ld;
particleFilter pf(100, 0.0f, 0.0f, 0.0f);  
driveOdometry driveOdom;
slamManager slamMan(&pf, &driveOdom, &map, &ld);

int main(){
    // slamMan.startUp();
    // for (int i = 0; i < 5; ++i) {
    //     slamMan.update();
    // }
    // return 0;
    while(true){
        stm32.send_Message("Test Message","non");
        sleep(5);
    }
    
}