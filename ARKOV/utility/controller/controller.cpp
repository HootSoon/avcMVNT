#include "controller.hh"
#include <fcntl.h>              // File Control , provides flags and functions for file operations
#include <unistd.h>             // Unix Standard , provides the api for accessing files
#include <linux/joystick.h>     // defines the memory layout of the joystick commands
#include <iostream>

// HR : We dont rlly want serial in this class 
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
// HR : We dont want any serial definitions in this class, that is what the microcontrollercom is for, also we have a format we must determine in how we send the controls,
// this would make our code less modular.

//Arm serial
serial::Serial mySerial2("/dev/ttyUSB0", 115200, serial::Timeout::simpleTimeout(1000));  
// HR : We dont want the arm serial in the controller code where its unaccessible by other source code. 
// HR : Also arm does not utilize regular serial communication, we use SDK/roarm-ik library, roarm-ik library will be modified to work on c++


// HR : The controller function is the constructor, it should not do anything else than the bare minimum of setting internal variables to the variables that have been brought in
// aka if we saved the port to a string inside the class then the input std::string port is to be set as the value of an internal std::string port. We need seperate functions for all the
// polling of the controller. 
//
// a "poll" function or some other function that once run starts saving the values from the controller is a more preferred design choice, the current design gives us no control over when 
// we want to start actually listening to controller inputs
//
// All these variables created in the constructor here are only local variables and not actual class variables because you did not define them in your header file. This means even if you did
// make a seperate poll function you also need to make sure that you make the variables that are needed between multiple functions and to be accessed outside the file must be defined in the header

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

            // HR : This might cause the code to crash if JS_EVENT_HAT does not exist
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

            // HR : This needs to be external , not in controller code itself, this ensure modular and safe code, we want pieces to be individuals and instead interface together

            // std::string cm = 
                //depending on decided drive control, we will either do a conditional of sending left stick values
                //to arduino and right stick values to arm, or we will send left and right stick values to arduino.
        }
        usleep(1000);
    }

    close(fd);

    // HR : because this function is a constructor and has no return of int type this will probably break your code 
    return 0;
}


