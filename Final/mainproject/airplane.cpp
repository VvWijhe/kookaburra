//
// Created by Jamie on 8-12-2016.
//
#include <airplane.h>
#include <stm32f0_discovery.h>

uint16_t currentPitch = 0;
uint16_t currentAltitude = 0;
uint16_t previousAltitude = 0;
float verticalSpeed = 0;
LEDColor_t ledColor = LEDRED;
uint32_t altitude1 = 0, altitude2 = 0;

MPU6050 Airplane::accelerometer;
MS5611 Airplane::barometer;

Airplane::Airplane() {
    // Enable flash acces
    //userData.init();

    // Initialize sensors
    accelerometer.init();
    barometer.initialize();

    // Initialize PWM
    control.init();

    // Initialize uart with interupts
    uart.init();

    // Initialize timers
    timer.setTim3(5);
    timer.setTim14(1);
    timer.setTim16(20);

    // Initialize input compare for the receiver

    // Set initiate flight parameters
    mode = MANUAL_M;

    // Test
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);
}

void Airplane::loop() {
    while (true) {
        while (mode == MANUAL_M) {
            // Read RC controller pwm and send to the motor and servos

            // Read RC controller pattern to activate autopilot

            // Test
            control.setOnTime(PWM_SERVO_RUDDER, 1500);
            control.setOnTime(PWM_SERVO_ELEVATOR, 1500);
            control.setOnTime(PWM_MOTOR, 1500);
        }

        while (mode == AUTOPILOT_M) {

            // Follow altitude 1 for 10 seconds
            while (currentAltitude != altitude1) {
                // ------------ Control Leds ------------
                // 1. Altitude is OK
                if (currentAltitude < altitude1 + 3 || currentAltitude > altitude1 - 3) {
                    ledColor = LEDGREEN;
                }

                // 2. Altitude is too high
                if (currentAltitude > altitude1 + 3) {
                    ledColor = LEDYELLOW;
                }

                // 2. Altitude is too low
                if (currentAltitude < altitude1 - 3) {
                    ledColor = LEDRED;
                }

                // ----------- Control motor ------------

                // ----------- Control servo ------------

            }

            // Follow altitude 2 for 10 seconds
            while (currentAltitude != altitude2) {
                if (currentAltitude < altitude2 + 3 && currentAltitude > altitude2 - 3) {
                    // Color->Green;
                }
                if (currentAltitude > altitude2 + 3) {
                    // Color->Orange;
                }
                if (currentAltitude < altitude2 - 3) {
                    // Color->Red;
                }
            }
            // control height

            // control pitch

        }
    }
}

uint32_t Airplane::getAltitude() {
    return (uint32_t) barometer.getAltitude();
}

uint32_t Airplane::getPitch() {
    const double CONVERSIONG = 3.9;
    accelGyroDataRaw_t accelGyroDataRaw;

    accelerometer.getRawAccelGyro(&accelGyroDataRaw);

    return (uint32_t) (accelGyroDataRaw.Ax * CONVERSIONG);
    //double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
    //double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);
}
