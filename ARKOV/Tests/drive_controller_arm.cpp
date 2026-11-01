#include "types/types.hh"
#include "communication/tests/microControllerCom.h"
#include "manipulation/roarm_ik.h"
#include "utility/controller/controller.hh"
#include <unistd.h>


controller controller("/dev/input/js0");
microcontroller stm32("/dev/ttyACM0", 115200);
roarm arm("/dev/ttyUSB0", 115200);
ik_solver armIK(&arm);


int main(){
    double timeDelay = 0.05;

    std::string received;

    Packet counts;

    controller.open();
    controller.startPolling();
    arm.poll_pose();

    while(true){

        // Get encoder serial packet & compute distances
        counts = stm32.parse_Serial(stm32.read_Serial(0));
        
        cout << arm.pose.get(0) << " , " << arm.pose.get(1) << " , " << arm.pose.get(2) << std::endl;

        if(controller.buttonValues.getIndex(0) == 0){
            stm32.send_Message("L:" + std::to_string(static_cast<int>(controller.stickValues.getIndex(1)/32767.0f * -100)) + " R:" + std::to_string(static_cast<int>(controller.stickValues.getIndex(3)/32767.0f * 100)),"NON");
        }else{
            stm32.send_Message("L:" + std::to_string(0) + " R:" + std::to_string(0),"NON");
            // phi, x, y, z, spd, acc
            armIK.move_to_xyz(0,((controller.stickValues.getIndex(1)/32767.0f * 6)+6), ((controller.stickValues.getIndex(1)/32767.0f * 12)), 10, 50, 10);
        }
        
        usleep(timeDelay*1000000);
    }
    
    
}

