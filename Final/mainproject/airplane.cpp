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
    // Initialize sensors
    accelerometer.init();
    barometer.initialize();

    // Initialize PWM
    control.init();

    // Initialize uart with interupts
    uart.init();

    // Initialize timers
    timer.setTim3(5);
    timer.setTim16(20);

    // Initialize input compare for the receiver

    // Initialize leds
    // GPIOC Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // PC8 and PC9 in output mode
    GPIOA->MODER |= (GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0) ;
    // Push pull mode selected
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_12) ;
    // Maximum speed setting
    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12);
    // Pull-up and pull-down resistors disabled
    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR11 | GPIO_PUPDR_PUPDR12);

    // Set initiate flight parameters
    mode = AUTOPILOT_M;

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
        }

        while (mode == AUTOPILOT_M) {
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

                // ----------- Control servo ------------

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

// Toggle GPIO ports for the leds
void Airplane::setColor(LEDColor_t Color){
    if(Color == LEDGREEN){
        GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        if(!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)){
            GPIO_SetBits(GPIOA, GPIO_Pin_11);
        }
        else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        }
        STM_EVAL_LEDToggle(LED3);

    }

    if(Color == LEDYELLOW){
        if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11) != GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12)){
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        else if(!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11)){
            GPIO_SetBits(GPIOA, GPIO_Pin_11);
            GPIO_SetBits(GPIOA, GPIO_Pin_12);
        }
        else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_11);
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        STM_EVAL_LEDToggle(LED3);
    }

    if(Color == LEDRED){
        GPIO_ResetBits(GPIOA, GPIO_Pin_11);
        if(!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12)){
            GPIO_SetBits(GPIOA, GPIO_Pin_12);
        }
        else {
            GPIO_ResetBits(GPIOA, GPIO_Pin_12);
        }
        STM_EVAL_LEDToggle(LED3);
    }
}
