#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <string>

class controller {
public:
    controller(std::string port = "/dev/input/js0");

private:
    std::string port;
    int fd;
    bool connected;

    short xlaxisVal;
    short ylaxisVal;

    short xraxisVal;
    short yraxisVal;

    bool modestate;
};


#endif