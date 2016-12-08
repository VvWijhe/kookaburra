//
// Created by Jamie on 10-11-2016.
//

#include "pid.h"

PID::PID(double stepTime, double p, double i, double d):
        dt(stepTime),
        kp(p),
        ki(i),
        kd(d)
{ }

double PID::calculate(double measuredValue, double setpoint) {
    double error = setpoint - measuredValue;

    // Proportional
    double Pout = kp * error;

    // Integral
    integral += error * dt;
    double Iout = ki * integral;

    // Derative
    double derative = (error - previousError) / dt;
    double Dout = kd * derative;

    previousError = error;

    return Pout + Iout + Dout;
}
