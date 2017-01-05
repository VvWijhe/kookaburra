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
LEDColor_t ledColor = LEDRED;
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
    timer.setTim17(5);

    // Initialize leds
    // GPIOC Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // PC8 and PC9 in output flightMode
    GPIOA->MODER |= (GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0);
    // Push pull flightMode selected
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_12);
    // Maximum speed setting
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12);
    // Pull-up and pull-down resistors disabled
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR11 | GPIO_PUPDR_PUPDR12);

    // Set initiate flight parameters
    flightMode = MANUAL_M;
    TIM_Cmd(TIM14, DISABLE);
    TIM_Cmd(TIM17, DISABLE);

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
     *
     *
     **************************************
     * */
void Airplane::loop() {
    while (true) {
        while (flightMode == MANUAL_M) {

        }

        while (flightMode == AUTOPILOT_M) {
            // Follow altitude 1 for 10 seconds
            while (Time::seconds < 10) {
                // ------------ Control Leds ------------
                // 1. Altitude is OK
                if (currentAltitude < altitude1 + 3 && currentAltitude > altitude1 - 3) {
                    ledColor = LEDGREEN;
                    //ENABLE TIM14
                    TIM_Cmd(TIM14, ENABLE);
                }

                // 2. Altitude is too high
                if (currentAltitude > altitude1 + 3) {
                    ledColor = LEDYELLOW;
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // 2. Altitude is too low
                if (currentAltitude < altitude1 - 3) {
                    ledColor = LEDRED;
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // ----------- Control motor ------------
                controlMotor(altitude1);

                // ----------- Control servo ------------
                controlElevator(MIN_ANGLE, MAX_ANGLE);
            }

            // Follow altitude 2 for 10 seconds
            while (Time::seconds < 20) {
                if (currentAltitude < altitude2 + 3 && currentAltitude > altitude2 - 3) {

                    ledColor = LEDGREEN;
                    //ENABLE TIM14
                    TIM_Cmd(TIM14, ENABLE);
                }
                if (currentAltitude > altitude2 + 3) {
                    ledColor = LEDYELLOW;
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }
                if (currentAltitude < altitude2 - 3) {
                    ledColor = LEDRED;
                    //DISABLE TIM14
                    TIM_Cmd(TIM14, DISABLE);
                }

                // ----------- Control motor ------------
                controlMotor(altitude2);

                // ----------- Control servo ------------
                controlElevator(MIN_ANGLE, MAX_ANGLE);
            }

            // Reset values
            TIM_Cmd(TIM14, DISABLE);
            Time::seconds = 0;
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

// Toggle GPIO ports for the leds
void Airplane::setColor(LEDColor_t Color) {
    if (Color == LEDGREEN) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        if (!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)) {
            GPIO_SetBits(GPIOA, GPIO_Pin_11);
        } else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        }
        STM_EVAL_LEDToggle(LED3);
    }

    if (Color == LEDYELLOW) {
        if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11) != GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12)) {
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        } else if (!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)) {
            GPIO_SetBits(GPIOA, GPIO_Pin_11);
            GPIO_SetBits(GPIOA, GPIO_Pin_12);
        } else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        STM_EVAL_LEDToggle(LED3);
    }

    if (Color == LEDRED) {
        GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        if (!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12)) {
            GPIO_SetBits(GPIOA, GPIO_Pin_12);
        } else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        STM_EVAL_LEDToggle(LED3);
    }
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

    // Set to PWM
    altitudeControlValue *= 0.4;

    if (altitudeControlValue < 0) altitudeControlValue = 0;
    if (altitudeControlValue > 2000) altitudeControlValue = 2000;

    control.setOnTime(PWM_MOTOR, (uint32_t) (1000 + altitudeControlValue));
}
