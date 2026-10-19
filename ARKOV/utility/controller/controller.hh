#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <string>
#include <thread>
#include <atomic>
class controller {
public:
    controller(std::string port = "/dev/input/js0");
    ~controller();
    std::string port;
    int fd;
    bool connected;

    short xlaxisVal;
    short ylaxisVal;

    short xraxisVal;
    short yraxisVal;

    bool modestate;
    bool drivestate;

    bool open();
    void close();
    void poll();

    bool startPolling();
    void stopPolling();
private:
    std::atomic<bool> pollCondition;
    std::thread pollThread;
};


#endif