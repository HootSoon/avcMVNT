#include <fcntl.h>              // File Control , provides flags and functions for file operations
#include <unistd.h>             // Unix Standard , provides the api for accessing files
#include <linux/joystick.h>     // defines the memory layout of the joystick commands
#include <iostream>

#include "serial/serial.h"      // Serial for microcontroller communication

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



// Serial Communication Setup
std::string message = "";
std::string baud = "COM13";
uint32_t baud = 115200;
serial::Serial serialCom(port, baud, serial::Timeout::simpleTimeout(1000));
// --------------------------



int main() {
    // | is a bitwise or , you have to combine the startup flags
    int fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        return 1;
    }

    struct js_event e;

    
    short xlaxisVal = 0;
    short ylaxisVal = 0;

    short xraxisVal = 0;
    short yraxisVal = 0;

    short button = 0;

    bool condition = true;


    while (condition) {
        while (read(fd, &e, sizeof(e)) > 0) {
            // At beginning of setup linux sends init information , we can ignore it, & is a bitwise and op
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
                std::cout << "L (" << xlaxisVal << " : " << ylaxisVal << ") R (" << xraxisVal << " : " << yraxisVal << ")" << std::endl;
                
                
            }
            if (e.type == JS_EVENT_BUTTON ) {
                button = e.number;
                std::cout << "Button Press: " << button << std::endl; 
            }
            
            if (e.type == JS_EVENT_HAT) {
                hat = e.number;
                std::cout << "Hat Press: " << hat << std::endl;
            }

/*          if (e.type == JS_EVENT_BUTTON && e.value == 0) {
                std::cout << "Closing" << std::endl;
                condition = false;
                break;
            } */

            // Temporary testing serial to send commands to motors
            if(serialCom.isOpen()){
                message << xlaxisVal << " " << ylaxisVal << " " << xraxisVal << " " << yraxisVal << " \n";
                serialCom.write(message);
                message = "";
            }
        }
        usleep(1000);
    }

    close(fd);
    return 0;
}