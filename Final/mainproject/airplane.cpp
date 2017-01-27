//
// Created by Jamie on 8-12-2016.
//
#include <airplane.h>
#include <pid.h>

int altitude1 = 0, altitude2 = 0;
double currentPitch = 0;
float currentAltitude = 0;
float previousAltitude = 0;
float verticalSpeed = 0;
flightMode_t flightMode = MANUAL_M;

MPU6050 accelerometer;
MS5611 barometer;

Airplane::Airplane() {
    // Initialize sensors
    accelerometer.init();
    barometer.initialize();

    // Initialize PWM and capture compare input
    control.init();
    control.InitCapComp();

    // Initialize serial with interrupts
    uart.init();

    // Initialize timers
    setTim3(5);
    setTim14(1);
    setTim16(20);

    // Initialize RGB led
    RGB::init();

    // Set initiate flight parameters
    flightMode = MANUAL_M;
    TIM_Cmd(TIM14, DISABLE);

    //writeFlash(120, 230);

    // Report sensor status via the usart
    uart << "Flightmode     : " << (flightMode == MANUAL_M ? "Manual" : "Autopilot") << "\n";
    //uart << "Altitude 1     : " << (uint32_t)readFlash(1) << "\n";
    //uart << "Altitude 2     : " << (uint32_t)readFlash(2) << "\n";
    uart << "Program initialized\n";
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
//            uart << "Altitude        : " << currentAltitude << "\n";
//            uart << "Pitch           : " << currentPitch << "\n";
        }

        while (flightMode == AUTOPILOT_M) {
            TIM_Cmd(TIM14, ENABLE);

            // --------------------------- 1 ------------------------------
            while (Time::seconds < FLIGHT_TIME1 && flightMode == AUTOPILOT_M) {
                // ------------ Control Leds ------------
                // 1. Altitude is OK
                if (currentAltitude < altitude1 + 3 && currentAltitude > altitude1 - 3) {
                    RGB::setColor(LEDGREEN);
                    //ENABLE TIM14
                    //TIM_Cmd(TIM14, ENABLE);
                }

                // 2. Altitude is too high
                if (currentAltitude > altitude1 + 3) {
                    RGB::setColor(LEDYELLOW);
                    //DISABLE TIM14
                    //TIM_Cmd(TIM14, DISABLE);
                }

                // 3. Altitude is too low
                if (currentAltitude < altitude1 - 3) {
                    RGB::setColor(LEDRED);
                    //DISABLE TIM14
                    //TIM_Cmd(TIM14, DISABLE);
                }

                // ----------- Control motor ------------
                controlMotor(altitude1);

                // ----------- Control servo ------------
                controlElevator(MIN_ANGLE, MAX_ANGLE);
            }

            // --------------------------- 2 ------------------------------
            while (Time::seconds < FLIGHT_TIME2 && flightMode == AUTOPILOT_M) {
                // ------------ Control Leds ------------
                // 1. Altitude is OK
                if (currentAltitude < altitude2 + 3 && currentAltitude > altitude2 - 3) {
                    RGB::setColor(LEDGREEN);
                    //ENABLE TIM14
                    //TIM_Cmd(TIM14, ENABLE);
                }

                // 2. Altitude is too high
                if (currentAltitude > altitude2 + 3) {
                    RGB::setColor(LEDYELLOW);
                    //DISABLE TIM14
                    //TIM_Cmd(TIM14, DISABLE);
                }

                // 3. Altitude is too low
                if (currentAltitude < altitude2 - 3) {
                    RGB::setColor(LEDRED);
                    //DISABLE TIM14
                    //TIM_Cmd(TIM14, DISABLE);
                }

                // ----------- Control motor ------------
                controlMotor(altitude2);

                // ----------- Control servo ------------
                controlElevator(MIN_ANGLE, MAX_ANGLE);
            }

            TIM_Cmd(TIM14, DISABLE);
            Time::seconds = 0;

            // --------------------------- 3 ------------------------------
            while(currentAltitude > 2 && flightMode == AUTOPILOT_M){
                control.setOnTime(PWM_MOTOR, 1300);
                controlElevator(0, MAX_ANGLE);
            }

            // Reset values
            RGB::disable();
        }
    }
}

float Airplane::getAltitude() {
    return barometer.getAltitude();
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
        PID pidPitch(0.02, 20.0, 0.0, 0.0);
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
        control.setOnTime(PWM_SERVO_ELEVATOR, (uint32_t) (1500 - pitchControlValue));
    } else {
        control.setOnTime(PWM_SERVO_ELEVATOR, (uint32_t) 1500);
    }
}

void Airplane::controlMotor(int setPoint) {
    PID pid(0.02, 160, 0, 0);
    double altitudeControlValue = 0;

    altitudeControlValue = pid.calculate(currentAltitude, setPoint);

    altitudeControlValue += 1300;

    if (altitudeControlValue < 1300) altitudeControlValue = 1300;
    if (altitudeControlValue > 2000) altitudeControlValue = 2000;

    control.setOnTime(PWM_MOTOR, (uint32_t) altitudeControlValue);
}
