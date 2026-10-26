#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <array>

class ThreadSafeArray{
    
    public:
    static constexpr int SIZE = 4;

    void setIndex(int index, short& value);
    void setAll(std::array<short,SIZE>& values);

    short getIndex(int index) const;
    std::array<short,SIZE> getAll() const;

    private:
    std::array<short,SIZE> stickValues;
    mutable std::mutex mtx;

};


class controller {
public:
    controller(std::string port = "/dev/input/js0");
    ~controller();
    std::string port;
    int fd;
    bool connected;

    bool modestate;
    bool drivestate;


    bool open();
    void close();
    void poll();

    bool startPolling();
    void stopPolling();

    ThreadSafeArray values;
private:
    std::atomic<bool> pollCondition;
    std::thread pollThread;
};



#endif