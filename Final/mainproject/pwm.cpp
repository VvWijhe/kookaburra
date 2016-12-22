//
// Created by Jamie on 10-11-2016.
//
#include <stm32f0xx_conf.h>
#include "pwm.h"

AirplaneControl::AirplaneControl() :
        isEnabled(false) {

}

void AirplaneControl::init() {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //(#) Configure the TIM pins by configuring the corresponding GPIO pins
    //    This is LED3 on STM32F0-Discovery
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_2);

    //(#) Configure the Timer base unit as described in the first part of this
    //    driver, if needed, else the Timer will run with the default
    //    configuration:
    //    (++) Autoreload value = 0xFFFF.
    //    (++) Prescaler value = 0x0000.
    //    (++) Counter flightMode = Up counting.
    //    (++) Clock Division = TIM_CKD_DIV1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / 1000000) - 1);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //(#) Fill the TIM_OCInitStruct with the desired parameters including:
    //    (++) The TIM Output Compare flightMode: TIM_OCMode.
    //    (++) TIM Output State: TIM_OutputState.
    //    (++) TIM Pulse value: TIM_Pulse.
    //    (++) TIM Output Compare Polarity : TIM_OCPolarity.
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //(#) Call TIM_OCxInit(TIMx, &TIM_OCInitStruct) to configure the desired
    //    channel with the corresponding configuration.
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    TIM_Cmd(TIM2, ENABLE);

    InitCapComp();

    isEnabled = true;
}

/// @brief PWM Capture Compare Init.
/// @param None
void AirplaneControl::InitCapComp() {
    // GPIOB Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // PB14 in input flightMode
    GPIOB->MODER |= (GPIO_MODER_MODER14_1);
    // Push pull flightMode selected
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_14);
    // Maximum speed setting
    GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR14);
    // Pull-up and pull-down resistors disabled
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR14);

    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* TIM15 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);

    /* GPIOB clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* TIM15 channel 1 configuration : PB.014 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect TIM pin to AF1 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_1);

    /* Enable the TIM15 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM15_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* ---------------------------------------------------------------------------
       TIM15 configuration: PWM Input flightMode
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
    TIM_SelectMasterSlaveMode(TIM15, TIM_MasterSlaveMode_Enable);

    /* TIM enable counter */
    TIM_Cmd(TIM15, ENABLE);

    /* Enable the CC1 Interrupt Request */
    TIM_ITConfig(TIM15, TIM_IT_CC1, ENABLE);
}

/// @brief sets pwm on time.
/// @param time: on time in ms
void AirplaneControl::setOnTime(pwmFunction_t function, uint32_t time) {
    if (function == PWM_SERVO_ELEVATOR && isEnabled) {
        TIM_SetCompare1(TIM2, time);
    } else if (function == PWM_SERVO_RUDDER && isEnabled) {
        TIM_SetCompare2(TIM2, time);
    } else if (function == PWM_MOTOR && isEnabled) {
        TIM_SetCompare3(TIM2, time);
    } else {
        while (1) {};
    }
}
