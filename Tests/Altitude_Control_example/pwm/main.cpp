#include <pwm.h>
#include <pid.h>

PWM pwm;

/// TBD: NEED BAROMETER

int main(void) {
    pwm.initMotor();

    while (1) {
        PID pid(0.02, 1, 0, 0);
        double altitudeControlValue = 0;
        double altitudeSetpoint = 0;

        altitudeControlValue = pid.calculate(barometerHeight, altitudeSetpoint);

        // Set to PWM
        altitudeControlValue *= 0.04;

        if(altitudeControlValue < 0) altitudeControlValue = 0;
        if(altitudeControlValue > 100) altitudeControlValue = 100;

        pwm.cycle(2, (uint32_t)altitudeControlValue);
    }
}

