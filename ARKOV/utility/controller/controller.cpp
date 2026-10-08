
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

}

void controller::poll() { 

}