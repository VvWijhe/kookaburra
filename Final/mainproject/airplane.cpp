//
// Created by Jamie on 8-12-2016.
//
#include <airplane.h>
#include <stm32f0_discovery.h>

uint16_t currentPitch = 0;
uint16_t currentAltitude = 0;

MPU6050 Airplane::accelerometer;
MS5611 Airplane::barometer;

Airplane::Airplane() {
    // Enable flash acces
    //userData.init();

    // Initialize sensors
    accelerometer.init();
    barometer.initialize();

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

//    userData.write32(ALT1_ADRESS, 1);
//    userData.write32(ALT2_ADRESS, 2);

    // Get user altitudes
//    altitude1 = userData.read32(ALT1_ADRESS);
//    altitude2 = userData.read32(ALT2_ADRESS);

    // Test
    STM_EVAL_LEDInit(LED4);
}

void Airplane::loop() {
    while (true) {
        while (mode == MANUAL_M) {
            // Read RC controller pwm and send to the motor and servos

            // Read RC controller pattern to activate autopilot

            // Test
            STM_EVAL_LEDOn(LED4);
        }

        while (mode == AUTOPILOT_M) {
            // control height

            // control pitch

        }
    }
}

uint32_t Airplane::getAltitude() {
    return barometer.getAltitude();
}

uint32_t Airplane::getPitch() {
    const double CONVERSIONG = 3.9;
    accelGyroDataRaw_t accelGyroDataRaw;

    accelerometer.getRawAccelGyro(&accelGyroDataRaw);

    return (uint32_t) (accelGyroDataRaw.Ax * CONVERSIONG);
    //double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
    //double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);
}
