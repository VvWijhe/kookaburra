#include <stm32f0xx_tim.h>
#include "h/counter.h"


Time::Time() {

}

void Time::init_Tim3(uint16_t p) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //[..] To use the Timer in Timing(Time base) mode, the following steps are
    //     mandatory:

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / (p * 1000)) - 1);

    //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure
    //    the Time Base unit with the corresponding configuration.
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

void Time::init_Tim16(uint16_t p) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //[..] To use the Timer in Timing(Time base) mode, the following steps are
    //     mandatory:

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);

    //(#) Fill the TIM_TimeBaseInitStruct with the desired parameters.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((SystemCoreClock / (p * 1000)) - 1);
<<<<<<< HEAD
=======
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
>>>>>>> b8e5ea2287f294661bac34dc4cff31b905eb106c

    //(#) Call TIM_TimeBaseInit(TIMx, &TIM_TimeBaseInitStruct) to configure
    //    the Time Base unit with the corresponding configuration.
    TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);

    //(#) Enable the NVIC if you need to generate the update interrupt.
    //    Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM16_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //(#) Enable the corresponding interrupt using the function
    //    TIM_ITConfig(TIMx, TIM_IT_Update).
    TIM_ITConfig(TIM16, TIM_IT_Update, ENABLE);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    TIM_Cmd(TIM16, ENABLE);
}

void Time::SetFreq16(float freq){
   Freq16 = (int)freq;
}

void Time::Raisetime(void) {
    Value++;

}

uint32_t Time ::GetValue(void) {
    return Value;
}

void Time::incrementTime(void) {

    if(Seconds++ == 60){
            if(Minutes++ == 60){
                if(Hours++ == 24){
                    Hours = 0;
                }
                Minutes = 0;
            }
            Seconds = 0;
    }



}

int Time::GetSeconds(void) {
    return (uint32_t)Seconds;
}

uint32_t Time::GetMinutes(void) {
    return Minutes;
}

int Time::GetHours(void){
    return Hours;
}


