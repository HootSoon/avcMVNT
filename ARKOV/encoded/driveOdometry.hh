#pragma once
#include "types.hh"

/*!
    \defgroup MOVEMENT
    @{
*/

class driveOdometry{
private:
    static constexpr float PI = 3.14159265f;
    static constexpr float DIAMETER = 0.25f;       
    static constexpr float WHEELBASE = 15.0f;      
    static constexpr float CIRCUMFERENCE = PI * DIAMETER;
    static constexpr int TICKS_PER_REVOLUTION = 2376;

    int lastTicsL = 0;
    int lastTicsR = 0;
    float rDist,lDist;
    float deltaDist;
    float deltaAngle;

    float totalDistanceTraveled;
    float velocity;
public:
    driveOdometry() = default; // makes compiler default constructor
    OdometryDelta calcDistanceTraveled(int curTicsL , int curTicsR, float deltaTime);
    
};
//! @}