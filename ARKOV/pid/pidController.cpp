#include "pidController.hh"
#include <algorithm>

// 10/31/2026
/*
    1. Modified integral portion to normalize min and max output (devide by k_i)
    2. Modified derivative portion to take derivative of velocity directly not error
*/

pidController::pidController(float p, float i, float d){
    k_p = p;
    k_i = i;
    k_d = d;
}

void pidController::setPoint(float newSetPoint){
    setpoint = newSetPoint;
}

void pidController::reset(){
    integral = 0.0f;
    error = 0.0f;
    prevValue = 0.0f;
}

float pidController::calculate(float value, float deltaT){
    if(deltaT <= 0.0f) { return 0.0f; } // this stops an explosion of the algorithm
    
    error = setpoint - value;
    
    integral += (error * deltaT);
    integral = std::clamp(integral, MIN_OUTPUT/k_i, MAX_OUTPUT/k_i);
    
    deriv = -(value - prevValue) / deltaT;
    prevValue = value;
    
    float output = (error * k_p) + (integral * k_i) + (deriv * k_d);
    return std::clamp(output, MIN_OUTPUT, MAX_OUTPUT); // makes sure output is within the max boundaries
}