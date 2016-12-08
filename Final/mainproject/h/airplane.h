//
// Created by Jamie on 8-12-2016.
//

#ifndef _AIRPLANE_H
#define _AIRPLANE_H

#include "mpu6050.h"
#include "usart.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint16_t currentPitch;
extern uint16_t currentAltitude;

class Airplane {
public:
    typedef enum {
        MANUAL_M,
        AUTOPILOT_M
    } flightMode_t;

    Airplane();

    void loop();

    static uint32_t getAltitude();

    static uint32_t getPitch();

private:
    static MPU6050 accelerometer;
    USART_1 usart;

    flightMode_t mode;
};

#ifdef __cplusplus
}
#endif

#endif //MAINPROJECT_AIRPLANE_H
