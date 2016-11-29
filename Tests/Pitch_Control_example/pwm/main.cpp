#include <stm32f0xx.h>
#include <pwm.h>
#include <cmath>
//#include <cmath>
#include "MPU6050.h"
#include "main.h"

#define CONVERSIONG 3.9

PWM pwm1;
MPU6050 accelerometer;

int main(void){
    pwm1.initServo();
    accelerometer.init();

    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);

    while(1){
        accelGyroDataRaw_t accelGyroDataRaw;

        if (accelerometer.testConnection()) {
            STM_EVAL_LEDToggle(LED3);
        } else {
            STM_EVAL_LEDToggle(LED4);
        }

        accelerometer.getRawAccelGyro(&accelGyroDataRaw);

        double accelerationX = (accelGyroDataRaw.Ax * CONVERSIONG);
        double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
        double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);

        double pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/M_PI;
        //double roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;
        //double yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;

        pitch += 90;

        pwm1.cycle(2, (uint32_t) ((1000 - 2000) * pitch / 180 + 1000) + 1000);
    }
}

