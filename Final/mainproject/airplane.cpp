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
    Airplane::CapCompInit(); //intitializes both the timer and the GPIO pin (B.014)

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

void Airplane::CapCompInit( void )
{
    // GPIOB Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // PB14 in input mode
    GPIOB->MODER |= (GPIO_MODER_MODER14_1) ;
    // Push pull mode selected
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_14) ;
    // Maximum speed setting
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR14);
    // Pull-up and pull-down resistors disabled
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR14);


    TIM_ICInitTypeDef  TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* TIM15 clock enable */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM15 , ENABLE );

    /* GPIOB clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* TIM15 channel 1 configuration : PB.014 */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect TIM pin to AF1 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);

    /* Enable the TIM15 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ---------------------------------------------------------------------------
       TIM15 configuration: PWM Input mode
        The external signal is connected to TIM15 CH1 pin (PB.014)
        TIM15 CCR2 is used to compute the frequency value
        TIM15 CCR1 is used to compute the duty cycle value

       External Signal Frequency = SystemCoreClock / TIM15_CCR2 in Hz.
       External Signal DutyCycle = (TIM15_CCR1*100)/(TIM15_CCR2) in %.
     Note:
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.
     --------------------------------------------------------------------------- */

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;

    TIM_PWMIConfig(TIM15, &TIM_ICInitStructure);

    /* Select the TIM15 Input Trigger: TI2FP2 */
    TIM_SelectInputTrigger(TIM15, TIM_TS_TI2FP2);

    /* Select the slave Mode: Reset Mode */
    TIM_SelectSlaveMode(TIM15, TIM_SlaveMode_Reset);
    TIM_SelectMasterSlaveMode(TIM15,TIM_MasterSlaveMode_Enable);

    /* TIM enable counter */
    TIM_Cmd(TIM15, ENABLE);

    /* Enable the CC1 Interrupt Request */
    TIM_ITConfig(TIM15, TIM_IT_CC1, ENABLE);
}