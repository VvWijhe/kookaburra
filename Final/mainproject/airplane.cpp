//
// Created by Jamie on 8-12-2016.
//
#include <airplane.h>
#include <stm32f0_discovery.h>
#include <pid.h>

int currentPitch = 0;
int currentAltitude = 0;
int previousAltitude = 0;
float verticalSpeed = 0;
int altitude1 = 0, altitude2 = 0;
flightMode_t flightMode = MANUAL_M;

MPU6050 Airplane::accelerometer;
MS5611 Airplane::barometer;

Airplane::Airplane() {
    // Initialize sensors
    accelerometer.init();
    barometer.initialize();

    // Initialize PWM and capture compare input
    control.init();

    // Initialize uart with interupts
    uart.init();

    // Initialize timers
    timer.setTim3(5);
    timer.setTim14(1);
    timer.setTim16(20);

    // Initialize RGB led
    RGB::init();

    // Set initiate flight parameters
    flightMode = MANUAL_M;
    TIM_Cmd(TIM14, DISABLE);

    // Test
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);
}

/***************************************
     * Start of the main loop there are 2 states:
     *
     * State 1 MANUAL:
     * Does nothing. Wait for user input.
     *
     * State 2 AUTOPILOT:
     * Follows the flightpath.
     *  1. Fly 10 seconds on altitude 1
     *  2. Fly 10 seconds on altitude 2
     *  3. Land
     *
     ************************************/
void Airplane::loop() {
    while (true) {
        while (flightMode == MANUAL_M) {

        }

        while (flightMode == AUTOPILOT_M) {
            // --------------------------- 1 ------------------------------
            while (Time::seconds < 10) {
                // ------------ Control Leds ------------
                // 1. Altitude is OK
                if (currentAltitude < altitude1 + 3 && currentAltitude > altitude1 - 3) {
                    RGB::setColor(LEDGREEN);
                    //ENABLE TIM14
                    TIM_Cmd(TIM14, ENABLE);
                }

                // 2. Altitude is too high
                if (currentAltitude > altitude1 + 3) {
                    RGB::setColor(LEDYELLOW);
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // 3. Altitude is too low
                if (currentAltitude < altitude1 - 3) {
                    RGB::setColor(LEDRED);
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // ----------- Control motor ------------
                controlMotor(altitude1);

                // ----------- Control servo ------------
                controlElevator(MIN_ANGLE, MAX_ANGLE);
            }

            // --------------------------- 2 ------------------------------
            while (Time::seconds < 20) {
                // ------------ Control Leds ------------
                // 1. Altitude is OK
                if (currentAltitude < altitude2 + 3 && currentAltitude > altitude2 - 3) {
                    RGB::setColor(LEDGREEN);
                    //ENABLE TIM14
                    TIM_Cmd(TIM14, ENABLE);
                }

                // 2. Altitude is too high
                if (currentAltitude > altitude2 + 3) {
                    RGB::setColor(LEDYELLOW);
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // 3. Altitude is too low
                if (currentAltitude < altitude2 - 3) {
                    RGB::setColor(LEDRED);
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // ----------- Control motor ------------
                controlMotor(altitude2);

                // ----------- Control servo ------------
                controlElevator(MIN_ANGLE, MAX_ANGLE);
            }

            // --------------------------- 2 ------------------------------

            // Reset values
            TIM_Cmd(TIM14, DISABLE);
            Time::seconds = 0;
            RGB::disable();
        }
    }
}

uint32_t Airplane::getAltitude() {
    return (uint32_t) barometer.getAltitude();
}

uint32_t Airplane::getPitch() {
    const double gConversion = 3.9;
    accelGyroDataRaw_t accelGyroDataRaw;

    accelerometer.getRawAccelGyro(&accelGyroDataRaw);

    return (uint32_t) (accelGyroDataRaw.Ax * gConversion);
    //double accelerationY = (accelGyroDataRaw.Ay * gConversion);
    //double accelerationZ = (accelGyroDataRaw.Az * gConversion);
}

void Airplane::controlElevator(int minAngle, int maxAngle) {
    if (currentPitch > maxAngle || currentPitch < minAngle) {
        PID pidPitch(0.02, 15.0, 0.0, 0.0);
        double pitchControlValue = 0;

        // Error and control value
        if (currentPitch > maxAngle) {
            pitchControlValue = pidPitch.calculate(currentPitch, maxAngle);
        } else {
            pitchControlValue = pidPitch.calculate(currentPitch, minAngle);
        }

        // Saturation
        if (pitchControlValue > 500) pitchControlValue = 495;
        else if (pitchControlValue < -500) pitchControlValue = -495;

        // Set servo
        control.setOnTime(PWM_SERVO_ELEVATOR, (uint32_t) (pitchControlValue + 1500));
    } else {
        control.setOnTime(PWM_SERVO_ELEVATOR, (uint32_t) 1500);
    }
}

void Airplane::controlMotor(int setPoint) {
    PID pid(0.02, 1, 0, 0);
    double altitudeControlValue = 0;

    altitudeControlValue = pid.calculate(currentAltitude, setPoint);

    altitudeControlValue *= 0.4;

    if (altitudeControlValue < 0) altitudeControlValue = 0;
    if (altitudeControlValue > 2000) altitudeControlValue = 2000;

    control.setOnTime(PWM_MOTOR, (uint32_t) (1000 + altitudeControlValue));
}
