#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "manipulation/roarm_ik.hh"
#include "utility/controller/controller.hh"
#include "utility/cameras/cameras.hh"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <algorithm>


/*
The Purpose of this test it to see how the driving, some basic arm control, camera feed, and AI 
Can work together.

11/23/2026 - HR
Needs AI added once adri gets that going
*/





controller controller("/dev/input/js0");
microcontroller stm32("/dev/ttyACM0", 115200);
roarm arm("/dev/ttyUSB0", 115200);
ik_solver armIK(&arm);
cv_webcam webcam(0); 




int main(){
    // Serial/Encoder Variables
    double timeDelay = 0.05;
    std::string received;
    Packet counts;
    //------------------


    // Arm pose variables
    double pose0 = 0.0;
    double pose1 = 0.0;

    double x = 6.0;
    double y = 0.0;
    double z = 10.0;
    //------------------


    // Cameras Variables
    bool read_Con;
    cv::Mat frame;
    //------------------

    controller.open();
    controller.startPolling();
    arm.poll_pose();

    while(true){


        // Get camera frame
        read_Con = cam.update_frame(frame);

        if(read_Con){
            cam.display_frame(frame);
        }

        // Get encoder serial packet & compute distances
        counts = stm32.parse_Serial(stm32.read_Serial(0));

        

        arm.pose.get(1,pose0);
        arm.pose.get(2,pose1);
        std::cout << pose0 << " , " << pose1 << " , " << x << " , " << y << " , " << z <<  std::endl;

        if(controller.buttonValues.getIndex(0) == 0){
            // Send speed commands to STM32 based on controller input
            stm32.send_Message("L:" + std::to_string(static_cast<int>(controller.stickValues.getIndex(3)/32767.0f * 100)) + " R:" + std::to_string(static_cast<int>(controller.stickValues.getIndex(1)/32767.0f * 100)),"NON");
        }else{
            // If button 0 is pressed, change arm target position with controller
            x += (controller.stickValues.getIndex(1)/32767.0f * -1);
            y += (controller.stickValues.getIndex(2)/32767.0f * -1);
            z += (controller.stickValues.getIndex(3)/32767.0f * 1);

            stm32.send_Message("L:" + std::to_string(0) + " R:" + std::to_string(0),"NON");
            // phi, x, y, z, spd, acc


            // Send arm to target position if button 1 is pressed
            if(controller.buttonValues.getIndex(1) == 1){
                armIK.move_to_xyz(0,(x), (y), z, 50, 10);
                usleep(timeDelay*20000000);
            }
            usleep(timeDelay*1000000);
        }
        
        usleep(timeDelay*1000000);
    }
    
    
}

