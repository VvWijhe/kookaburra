//
// Created by Jamie on 10-11-2016.
//

#ifndef MAINPROJECT_PID_H
#define MAINPROJECT_PID_H


#include <cmath>

class PID {
public:
    PID(double stepTime, double p, double i, double d);
    double calculate(double er, double setp);

private:
    double dt;
    double previousError;
    double integral;

    double kp;
    double ki;
    double kd;
};


#endif //MAINPROJECT_PID_H
