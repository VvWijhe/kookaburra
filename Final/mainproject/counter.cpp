#include <stm32f0xx_tim.h>
#include "h/counter.h"

int Time::hours = 0;
int Time::minutes = 0;
int Time::seconds = 0;

Timer::Timer() {

}

void Timer::setTim3(uint16_t p) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_DeInit(TIM3);

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / (p * 1000)) - 1);

    //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure
    //    the Timer Base unit with the corresponding configuration.
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    //(#) Enable the NVIC if you need to generate the update interrupt.
    //    Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //(#) Enable the corresponding interrupt using the function
    //    TIM_ITConfig(TIMx, TIM_IT_Update).
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    TIM_Cmd(TIM3, ENABLE);
}

void Timer::setTim14(uint16_t p) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_DeInit(TIM14);

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

    //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / (p * 1000)) - 1);

    //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure
    //    the Timer Base unit with the corresponding configuration.
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);

    //(#) Enable the NVIC if you need to generate the update interrupt.
    //    Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //(#) Enable the corresponding interrupt using the function
    //    TIM_ITConfig(TIMx, TIM_IT_Update).
    TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.

}

void Timer::setTim16(uint16_t p) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_DeInit(TIM16);

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / (p * 1000)) - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure
    //    the Timer Base unit with the corresponding configuration.
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

    //(#) Enable the NVIC if you need to generate the update interrupt.
    //    Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //(#) Enable the corresponding interrupt using the function
    //    TIM_ITConfig(TIMx, TIM_IT_Update).
    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    TIM_Cmd(TIM16, ENABLE);
}

void Timer::setTim17(uint16_t p) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    TIM_DeInit(TIM17);

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

    //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / (p * 2000)) - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure
    //    the Timer Base unit with the corresponding configuration.
    TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);

    //(#) Enable the NVIC if you need to generate the update interrupt.
    //    Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM17_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //(#) Enable the corresponding interrupt using the function
    //    TIM_ITConfig(TIMx, TIM_IT_Update).
    TIM_ITConfig(TIM17, TIM_IT_Update, ENABLE);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    TIM_Cmd(TIM17, ENABLE);
}

void Timer::incrementTime(int &seconds) {
    seconds++;
}
