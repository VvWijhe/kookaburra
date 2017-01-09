//
// Created by Jamie on 8-12-2016.
//

#ifndef _AIRPLANE_H
#define _AIRPLANE_H

#define MAX_ANGLE 10
#define MIN_ANGLE -10

#include "mpu6050.h"
#include "usart.h"
#include "pwm.h"
#include "MS5611.h"
#include "counter.h"
#include "rgbdriver.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MANUAL_M,
    AUTOPILOT_M
} flightMode_t;

// Global variables, used in interrupt handlers
extern int currentPitch;
extern int currentAltitude;
extern int previousAltitude;
extern int altitude1, altitude2;
extern float verticalSpeed;
extern flightMode_t flightMode;
extern ppmData_t ppmData;

class Airplane {
public:

    Airplane();

    void loop();

    // Called in timer interrupt
    static uint32_t getAltitude();

    static uint32_t getPitch();

    void controlElevator(int minAngle, int maxAngle);

    void controlMotor(int setPoint);

private:
    AirplaneControl control;
    static MPU6050 accelerometer;
    static MS5611 barometer;
};

#ifdef __cplusplus
}
#endif

#endif //MAINPROJECT_AIRPLANE_H
