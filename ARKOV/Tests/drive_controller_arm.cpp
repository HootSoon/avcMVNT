#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "manipulation/roarm_ik.hh"
#include "utility/controller/controller.hh"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <algorithm>


controller controller("/dev/input/js0");
microcontroller stm32("/dev/ttyACM0", 115200);
roarm arm("/dev/ttyUSB0", 115200);
ik_solver armIK(&arm);


int main(){
    double timeDelay = 0.05;

    std::string received;

    Packet counts;

    double pose0 = 0.0;
    double pose1 = 0.0;

    double x = 6.0;
    double y = 0.0;
    double z = 10.0;

    controller.open();
    controller.startPolling();
    arm.poll_pose();

    while(true){

        // Get encoder serial packet & compute distances
        counts = stm32.parse_Serial(stm32.read_Serial(0));

        

        arm.pose.get(1,pose0);
        arm.pose.get(2,pose1);
        std::cout << pose0 << " , " << pose1 << " , " << x << " , " << y << " , " << z <<  std::endl;

        if(controller.buttonValues.getIndex(0) == 0){
            stm32.send_Message("L:" + std::to_string(static_cast<int>(controller.stickValues.getIndex(1)/32767.0f * -100)) + " R:" + std::to_string(static_cast<int>(controller.stickValues.getIndex(3)/32767.0f * 100)),"NON");
        }else{
            x += (controller.stickValues.getIndex(1)/32767.0f * -1);
            y += (controller.stickValues.getIndex(2)/32767.0f * -1);
            z += (controller.stickValues.getIndex(3)/32767.0f * 1);

            stm32.send_Message("L:" + std::to_string(0) + " R:" + std::to_string(0),"NON");
            // phi, x, y, z, spd, acc
            if(controller.buttonValues.getIndex(1) == 1){
                armIK.move_to_xyz(0,(x), (y), z, 50, 10);
                usleep(timeDelay*20000000);
            }
            usleep(timeDelay*1000000);
        }
        
        usleep(timeDelay*1000000);
    }
    
    
}

