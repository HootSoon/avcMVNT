
#include "controller.hh"
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <iostream>
#include <thread>
#include <atomic>


controller::controller(std::string port) {
    this->port = port; //note to self: this->port = port allows use of the same variable name for the class member and constructor parameter.
    fd = -1;
    connected = false;
    pollCondition = false;

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
    stopPolling();
    
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

    while (pollCondition.load()) {
        if(read(fd, &e, sizeof(e)) > 0){

            if (e.type & JS_EVENT_INIT) {
                continue;
            }
            
            if (e.type == JS_EVENT_AXIS ) {
                // Switch statement could actually be replaced, using just e.number directly to set index
                switch(e.number){
                    case 0:
                        values.setIndex(0,e.value);
                        break;
                    case 1:
                        values.setIndex(1,e.value);
                        break;
                    case 2:
                        values.setIndex(2,e.value);
                        break;
                    case 3:
                        values.setIndex(3,e.value);
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
        usleep(1000);
    }
}
bool controller::startPolling() {
    if (!connected || fd < 0 || pollThread.joinable()) { 
        return false;                                    
    }                                                    

    pollCondition.store(true);
    pollThread = std::thread(&controller::poll, this);
    return true;
}

void controller::stopPolling() {
    pollCondition.store(false);

    if (pollThread.joinable()) {
        pollThread.join();
    }
}


controller::~controller() {
    stopPolling();
    close(); 
}


// ThreadSafe Variable Definitions


short ThreadSafeArray::getIndex(int index) const{
    std::lock_guard<std::mutex> lock(mtx);
    return stickValues.at(index);
}

std::array<short,ThreadSafeArray::SIZE> ThreadSafeArray::getAll() const{
    std::lock_guard<std::mutex> lock(mtx);
    return stickValues;
}

void ThreadSafeArray::setIndex(int index, short& value){
    std::lock_guard<std::mutex> lock(mtx);
    stickValues.at(index) = value;
}

void ThreadSafeArray::setAll(std::array<short,SIZE>& values){
    std::lock_guard<std::mutex> lock(mtx);
    stickValues = values;
}