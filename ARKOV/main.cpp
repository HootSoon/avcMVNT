#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "pid/pidController.hh"
#include "slam/slamManager.hh"
#include "voxelVisulization/voxel_map.hh"
#include "utility/lidar/lidar.hh"
#include "utility/cameras/cameras.hh"
#include "localization/particleFilter.hh"
#include "encoded/driveOdometry.hh"
//#include "utility/controller/controller.hh"
#include <unistd.h>


//controller controller("/dev/input/js0");
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
    double timeDelay = 0.0125;

    std::string received;

    Packet counts;

    OdometryDelta d;

    float speedL = 0.0f;
    float speedR = 0.0f;
    //----------------------------------------

    leftPID.setPoint(5);
    rightPID.setPoint(5);

    while(true){
        received = stm32.read_Serial(0);
        counts = stm32.parse_Serial(stm32.read_Serial(0));

        d = driveOdom.calcDistanceTraveled(counts.leftCount, counts.rightCount);
        
        speedL = leftPID.calculate(static_cast<float>(d.deltaDist/timeDelay),timeDelay);
        speedR = rightPID.calculate(static_cast<float>(d.deltaDist/timeDelay),timeDelay);

        std::cerr << "Distance : " << d.deltaDist << " Angle : " << d.deltaAngle << " Counts : " << leftCount << " , " << rightCount << std::endl;

        stm32.send_Message("L:" + std::to_string(speedL) + " R:" + std::to_string(speedR),"DMA");

        sleep(timeDelay);
    }
    
}