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

double PID::calculate(double er, double setp) {
    return 0;
}
