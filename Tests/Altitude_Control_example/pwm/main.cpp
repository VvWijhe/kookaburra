#include <pwm.h>
#include <pid.h>

#define ALT_SETPOINT 10

PWM pwm;

/// TBD: NEED BAROMETER

int main(void) {
    pwm.initMotor();

    while (1) {
        PID pid(0.02, 1, 0, 0);
        double altitudeControlValue;

        altitudeControlValue = pid.calculate(barometerHeight, ALT_SETPOINT);

        // Set to PWM
        altitudeControlValue *= 0.04;

        if(altitudeControlValue < 0) altitudeControlValue = 0;
        if(altitudeControlValue > 100) altitudeControlValue = 100;

        pwm.cycle(2, (uint32_t)altitudeControlValue);
    }
}

