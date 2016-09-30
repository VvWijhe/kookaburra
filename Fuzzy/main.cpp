#include <iostream>
#include "fuzzy.h"

// Fuzzy class variable
Fuzzy fuzzy;

// Fuzzy member functions
t_er er;
t_cv cv;

// Difference equation variables for simulation
double setPoint = 2048;
double tau = 0.47;
double tsample = 0.005;

double measuredVoltage = 0;
double controlVoltage = 0;

int main() {
    // Error MSF
    er.mbf_negL[0] = -4095;
    er.mbf_negL[1] = -4095;
    er.mbf_negL[2] = -2048;
    er.mbf_negL[3] = -1024;

    er.mbf_neg[0] = -2048;
    er.mbf_neg[1] = -1024;
    er.mbf_neg[2] = -1024;
    er.mbf_neg[3] = 0;

    er.mbf_no[0] = -1024;
    er.mbf_no[1] = 0;
    er.mbf_no[2] = 0;
    er.mbf_no[3] = 1024;

    er.mbf_pos[0] = 0;
    er.mbf_pos[1] = 1024;
    er.mbf_pos[2] = 1024;
    er.mbf_pos[3] = 2048;

    er.mbf_posL[0] = 1024;
    er.mbf_posL[1] = 2048;
    er.mbf_posL[2] = 4095;
    er.mbf_posL[3] = 4095;

    // Control MSF
    cv.mbf_negL[0] = 0;
    cv.mbf_negL[1] = 685;
    cv.mbf_negL[2] = 1396;

    cv.mbf_neg[0] = 685;
    cv.mbf_neg[1] = 1396;
    cv.mbf_neg[2] = 2050;

    cv.mbf_no[0] = 1396;
    cv.mbf_no[1] = 2050;
    cv.mbf_no[2] = 2734;

    cv.mbf_pos[0] = 2050;
    cv.mbf_pos[1] = 2734;
    cv.mbf_pos[2] = 3419;

    cv.mbf_posL[0] = 2734;
    cv.mbf_posL[1] = 3419;
    cv.mbf_posL[2] = 4095;

    fuzzy.setMBF(cv);
    fuzzy.setMBF(er);

    for(int i = 0; i < 1000; i++){
        controlVoltage = fuzzy.process((int32_t)setPoint, (int32_t)measuredVoltage);

        measuredVoltage = measuredVoltage + (controlVoltage - measuredVoltage) * tsample / tau;

        std::cout << "Setpoint:         " << setPoint << std::endl;
        std::cout << "Measured:         " << measuredVoltage << std::endl;
        std::cout << "Control value:    " << controlVoltage << std::endl;
        std::cout << "------------------" << std::endl;
    }
    return 0;
}