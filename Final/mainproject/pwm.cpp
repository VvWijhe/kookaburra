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
    //    (++) Counter mode = Up counting.
    //    (++) Clock Division = TIM_CKD_DIV1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / 1000000) - 1);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //(#) Fill the TIM_OCInitStruct with the desired parameters including:
    //    (++) The TIM Output Compare mode: TIM_OCMode.
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

    isEnabled = true;
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

