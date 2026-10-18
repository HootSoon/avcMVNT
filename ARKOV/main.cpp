#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "pid/pidController.hh"
#include "slam/slamManager.hh"
#include "voxelVisulization/voxel_map.hh"
#include "utility/lidar/lidar.hh"
#include "utility/cameras/cameras.hh"
#include "localization/particleFilter.hh"
#include "encoded/driveOdometry.hh"
#include "utility/controller/controller.hh"
#include <unistd.h>

controller controller("/dev/input/js0");
microcontroller stm32("/dev/ttyACM0", 115200);
pidController leftPID(0.30, 0.05, 0.10);
pidController rightPID(0.30, 0.05, 0.10);
VoxelGrid map(100, 100, 50, 0.1);          
lidar ld("/dev/ttyACM0");
particleFilter pf(100, 0.0f, 0.0f, 0.0f);  
driveOdometry driveOdom;
// slamManager slamMan(&pf, &driveOdom, &map, &ld);

int main(){
    // slamMan.startUp();
    // for (int i = 0; i < 5; ++i) {
    //     slamMan.update();
    // }
    // return 0;

    


    // Just some temporary variables for test
    //----------------------------------------
    double timeDelay = 1.0;

    std::string received;

    int leftCount;
    int rightCount;

    OdometryDelta d;
    //----------------------------------------

    while(true){
        leftPID.setPoint(5);
        rightPID.setPoint(5);
        
        received = stm32.read_Serial(0)
        for(int i = 0; i < received.size(); i++){
            if(received.at(i) == ':'){
                leftCount = received.substr(0,i);
                rightCount = received.substr(i+1);
                break;
            }
        }
        d = calcDistanceTraveled(leftCount, rightCount);

        leftPID.calculate(d.deltaDist/timeDelay);
        rightPID.calculate(d.deltaDist/timeDelay);

        stm32.send_Message("L:50 R:50","non");

        sleep(timeDelay);
    }
    
}