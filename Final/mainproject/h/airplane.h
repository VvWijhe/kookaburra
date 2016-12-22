//
// Created by Jamie on 8-12-2016.
//

#ifndef _AIRPLANE_H
#define _AIRPLANE_H

#define MAX_ANGLE 10
#define MIN_ANGLE -10

// Include self made drivers
#include "mpu6050.h"
#include "usart.h"
#include "pwm.h"
#include "MS5611.h"
#include "counter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LEDGREEN,
    LEDRED,
    LEDYELLOW
}LEDColor_t;

typedef enum {
    MANUAL_M,
    AUTOPILOT_M
} flightMode_t;

// Global variables, used in interrupt handlers
extern uint16_t currentPitch;
extern uint16_t currentAltitude;
extern uint16_t previousAltitude;
extern uint32_t altitude1, altitude2;
extern float verticalSpeed;
extern LEDColor_t ledColor;
extern flightMode_t flightMode;

class Airplane {
public:

    Airplane();

    void loop();

    // Called in timer interrupt
    static uint32_t getAltitude();

    static uint32_t getPitch();

    static void setColor(LEDColor_t Color);

    void controlElevator(int minAngle, int maxAngle);

    void controlMotor(uint32_t setPoint);

private:
    UART uart;
    Timer timer;

    AirplaneControl control;
    static MPU6050 accelerometer;
    static MS5611 barometer;
};

#ifdef __cplusplus
}
#endif

#endif //MAINPROJECT_AIRPLANE_H
