#include <stm32f0xx.h>
#include <pwm.h>
#include <cmath>
#include <stm32f0_discovery.h>
#include <pid.h>
#include "MPU6050.h"

#define  MAX_ANGLE 20
#define  MIN_ANGLE -20
#define CONVERSIONG 3.9

PWM pwm;
MPU6050 accelerometer;

int main(void) {
    pwm.initServo();
    accelerometer.init();

    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);

    while (1) {
        accelGyroDataRaw_t accelGyroDataRaw;

        if (accelerometer.testConnection()) {
            STM_EVAL_LEDToggle(LED4);
        } else {
            STM_EVAL_LEDToggle(LED3);
        }

        accelerometer.getRawAccelGyro(&accelGyroDataRaw);

        double accelerationX = (accelGyroDataRaw.Ax * CONVERSIONG);
        double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
        double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);

        __volatile double pitch =
                180 * atan(accelerationX / sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) / M_PI;
        //double roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;
        //double yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;
        //pitch += 90;

        if (pitch > MAX_ANGLE || pitch < MIN_ANGLE) {
            PID pidPitch(0.02, 15, 0, 0);
            double pitchControlValue = 0;

            // Error an control value
            if (pitch > MAX_ANGLE) {
                pitchControlValue = pidPitch.calculate(pitch, MAX_ANGLE);
            } else {
                pitchControlValue = pidPitch.calculate(pitch, MIN_ANGLE);
            }
            //pitchControlValue *= 0.1;

            // Saturation
            if (pitchControlValue > 500) pitchControlValue = 495;
            else if (pitchControlValue < -500) pitchControlValue = -495;

            // Set servo
            pwm.cycle(2, (uint32_t) (pitchControlValue + 1500));
        } else {
            pwm.cycle(2, (uint32_t) 1500);
        }
    }
}

