
#include "controller.hh"
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <iostream>


controller::controller(std::string port) {
    this->port = port; //note to self: this->port = port allows use of the same variable name for the class member and constructor parameter.
    fd = -1;
    connected = false;

    xlaxisVal = 0;
    ylaxisVal = 0;

    xraxisVal = 0;
    yraxisVal = 0;

    modestate = false;
    drivestate = false;
}

bool controller::open() {
    fd = ::open(port.c_str(), O_RDONLY | O_NONBLOCK);

    if (fd < 0)
    {
        std::cerr << "Controller not found" << std::endl;
        connected = false;
        return false;
    }
    else
    {
        std::cout << "Controller connected" << std::endl;
        connected = true;
        return true;
    }
}

void controller::close() {
    if (fd >=0){
        fd = -1;
    }    
    connected = false;
}

void controller::poll() { 
    if (!connected || fd < 0) {
        return;
    }

    js_event e;

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
                        modestate = !modestate;
                    }
                    break;
                case 1:                    
                    if (e.value == 1){
                        drivestate = !drivestate;
                    }
                    break;
            }
        }
    }
}