#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "pid/pidController.hh"
#include "encoded/driveOdometry.hh"
#include "utility/controller/controller.hh"
#include <unistd.h>


controller controller("/dev/input/js0");
microcontroller stm32("/dev/ttyACM0", 115200);
pidController leftPID(40.0, 0.1, 0.01);
pidController rightPID(40.0, 0.1, 0.01);
driveOdometry driveOdom;
// slamManager slamMan(&pf, &driveOdom, &map, &ld);


int main(){
    double timeDelay = 0.05;

    std::string received;

    Packet counts;

    OdometryDelta d;

    float speedL = 0.0f;
    float speedR = 0.0f;

    float pidSetL = 0;
    float pidSetR = 0;

    controller.open();
    controller.startPolling();

    while(true){
        pidSetL += controller.values.getIndex(1)/32767.0f;
        pidSetR += controller.values.getIndex(3)/32767.0f;

        // SET TARGET VELOCITY for pid algorithm
        leftPID.setPoint(pidSetL);
        rightPID.setPoint(pidSetR);

        // Get encoder serial packet & compute distances
        counts = stm32.parse_Serial(stm32.read_Serial(0));
        d = driveOdom.calcDistanceTraveled(counts.leftCount, counts.rightCount, timeDelay);
        
        // Get speed commands from PID
        speedL = leftPID.calculate(static_cast<float>(d.leftVelocity),timeDelay);
        speedR = rightPID.calculate(static_cast<float>(d.rightVelocity),timeDelay) * -1;

        std::cerr << "PID : " << pidSetL << " , " << pidSetR << " Velocity : " << (d.leftVelocity + d.rightVelocity)/2.0f << " Counts : " << counts.leftCount << " , " << counts.rightCount << " Controller : " << speedL << " : " << speedR << std::endl;

        //stm32.send_Message("L:" + std::to_string(static_cast<int>(controller.values.getIndex(1)/32767.0f * -100)) + " R:" + std::to_string(static_cast<int>(controller.values.getIndex(3)/32767.0f * 100)),"NON");
        stm32.send_Message("L:" + std::to_string(static_cast<int>(speedL)) + " R:" + std::to_string(static_cast<int>(speedR)),"NON");

        usleep(timeDelay*1000000);
    }
    
    
}

