//
// Created by Jamie on 8-12-2016.
//
#include <airplane.h>
#include "airplane.h"

uint16_t currentPitch = 0;
uint16_t currentAltitude = 0;

int userAltitude1 = 0;
int userAltitude2 = 0;

MPU6050 Airplane::accelerometer;

Airplane::Airplane() {
    // Initialize sensors
    accelerometer.init();

    // Initialize motor
    control.initMotor();

    // Initialize servos
    control.initServo();

    // Initialize uart with interupts
    uart.init();

    // Initialize clockwork timer

    // Initialize input compare for the receiver

    // Set initiate flight parameters
    mode = MANUAL_M;
    userAltitude1 = 0;
    userAltitude2 = 0;
}

void Airplane::loop() {
    while (true) {
        while (mode == MANUAL_M) {
            // Read RC controller pwm and send to the motor and servos

            // Read RC controller pattern to activate autopilot

        }

        while (mode == AUTOPILOT_M) {
            // control height

            // control pitch

        }
    }
}

uint32_t Airplane::getAltitude() {
    return 0;
}

uint32_t Airplane::getPitch() {
    const double CONVERSIONG = 3.9;
    accelGyroDataRaw_t accelGyroDataRaw;

    accelerometer.getRawAccelGyro(&accelGyroDataRaw);

    return (uint32_t) (accelGyroDataRaw.Ax * CONVERSIONG);
    //double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
    //double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);
}
