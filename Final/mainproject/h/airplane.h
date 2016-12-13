//
// Created by Jamie on 8-12-2016.
//

#ifndef _AIRPLANE_H
#define _AIRPLANE_H

// Include self made drivers
#include "mpu6050.h"
#include "usart.h"
#include "pwm.h"
#include "flash.h"

#define ALT1_ADRESS EEPROM_START_ADDRESS
#define ALT2_ADRESS (EEPROM_START_ADDRESS + 1)

#ifdef __cplusplus
extern "C" {
#endif

// Global variables, used in interrupt handlers
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

    // Called in timer interrupt
    static uint32_t getAltitude();

    static uint32_t getPitch();

private:
    UART uart;
    Flash userData;

    AirplaneControl control;
    static MPU6050 accelerometer;

    flightMode_t mode;
    uint32_t altitude1, altitude2;
};

#ifdef __cplusplus
}
#endif

#endif //MAINPROJECT_AIRPLANE_H
