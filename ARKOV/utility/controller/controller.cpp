#include "controller.hh"
#include <fcntl.h>              // File Control , provides flags and functions for file operations
#include <unistd.h>             // Unix Standard , provides the api for accessing files
#include <linux/joystick.h>     // defines the memory layout of the joystick commands
#include <iostream>
#include <serial/serial.h>
// Possible values of type
//#define JS_EVENT_BUTTON         0x01    /* button pressed/released */
//#define JS_EVENT_AXIS           0x02    /* joystick moved */
//#define JS_EVENT_INIT           0x80    /* initial state of device */

// Struct definition for events
//struct js_event {
//        __u32 time;     /* event timestamp in milliseconds */
//        __s16 value;    /* value */
//        __u8 type;      /* event type */
//        __u8 number;    /* axis/button number */
//};

//Arduino serial
serial::Serial mySerial("/dev/ttyACM0", 115200, serial::Timeout::simpleTimeout(1000));
//Arm serial
serial::Serial mySerial2("/dev/ttyUSB0", 115200, serial::Timeout::simpleTimeout(1000));

controller::controller(std::string port){
    int fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
    
    if (fd < 0){
        std::cerr << "Failed to find controller" << std::endl;
    } else {
        std::cout << "Controller connected" << std::endl;
    }

    struct js_event e;

    short xlaxisVal = 0;
    short ylaxisVal = 0;

    short xraxisVal = 0;
    short yraxisVal = 0;

    short button = 0;

    bool condition = true;

    bool modestate = false;

    while (condition) {
        while (read(fd, &e, sizeof(e)) > 0) {
            if (e.type & JS_EVENT_INIT) {
                continue;
            }

            if (e.type == JS_EVENT_AXIS ) {
                switch(e.number){
                    case 0:
                        xlaxisVal = e.value;
                        break;
                    case 1:
                        ylaxisVal = e.value;
                        break;
                    case 2:
                        xraxisVal = e.value;
                        break;
                    case 3:
                        yraxisVal = e.value;
                        break;
                    default:
                        std::cout << "Other axis than 0-4" << std::endl;
                }
                
            }
            if (e.type == JS_EVENT_BUTTON ) {
                switch(e.number){
                    case 0:
                        if (e.value == 1){
                            modestate = modestate!;
                        }
                        break;
                    case 1:
                        drivestate = e.value;
                        break;
                    case 3:
                        unassigned = e.value;
                        break;
                    case 4:
                        unassigned = e.value;
                        break;
                    case 6:
                        flashbang = e.value;
                        break;
                    case 7:
                        grenade = e.value;
                        break;
                    default:
                        std::cout << "Other button than 0-7" << std::endl;
                }
            }
            //not sure what the hat values are, testing for that will be done later this week. 
            if(e.type == JS_EVENT_HAT){
                switch(e.number){
                    case 0:
                        uparrow = e.value;
                        break;
                    case 1:
                        downarrow = e.value;
                        break;
                    case 2:
                        leftarrow = e.value;        
                        break;
                    case 3:
                        rightarrow = e.value;
                        break:
                    default:
                        std::cout << "Other hat button than 0-3" << std::endl;
                }
            }
            // std::string cm = 
                //depending on decided drive control, we will either do a conditional of sending left stick values
                //to arduino and right stick values to arm, or we will send left and right stick values to arduino.
        }
        usleep(1000);
    }

    close(fd);
    return 0;
}


