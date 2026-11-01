#ifndef CONTROLLER_HH
#define CONTROLLER_HH

#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <array>

class ThreadSafeShortArray{
    
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

class ThreadSafeBoolArray{
    
    public:
    static constexpr int SIZE = 4;

    void setIndex(int index, bool& value);
    void setAll(std::array<bool,SIZE>& values);

    bool getIndex(int index) const;
    std::array<bool,SIZE> getAll() const;

    private:
    std::array<bool,SIZE> buttonValues;
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

    ThreadSafeShortArray stickValues;
    ThreadSafeBoolArray buttonValues;
private:
    std::atomic<bool> pollCondition;
    std::thread pollThread;
};



#endif