//
// Created by Jamie on 5-1-2017.
//

#include <stm32f0xx_gpio.h>
#include "rgbdriver.h"

void RGB::init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    // Init PWM gpio
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);

    // Init shift register gpio
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = LATCH_PIN | CLOCK_PIN | DATA_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);

    GPIO_ResetBits(GPIOA, CLOCK_PIN);
    GPIO_ResetBits(GPIOA, DATA_PIN);
    GPIO_ResetBits(GPIOA, LATCH_PIN);
    disable();
}

void RGB::setFrequency(float pulse) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = (uint32_t) (((1 / pulse) * 400) - 1);
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) (20000 - 1);
    TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = (uint32_t) 20;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM17, &TIM_OCInitStructure);

    TIM_Cmd(TIM17, ENABLE);
}

/**
 * @brief Qh is the 8th bit so, Qa = Red1, Qb = Green1
 * @param color
 */
void RGB::setColor(LEDColor_t color) {
    switch (color) {
        case LEDGREEN:
            setShiftRegister(0b00000010);
            break;
        case LEDRED:
            setShiftRegister(0b00000001);
            break;
        case LEDYELLOW:
            setShiftRegister(0b00000011);
            break;
    }
}

void RGB::disable() {
    // Turn off shift register
    setShiftRegister(0);
}

void RGB::setShiftRegister(uint8_t data) {
    GPIO_ResetBits(GPIOA, LATCH_PIN);

    for (int i = 8; i < 0; i--) {
        GPIO_ResetBits(GPIOA, CLOCK_PIN);

        if (data & (1 << i)) GPIO_SetBits(GPIOA, DATA_PIN);
        else GPIO_ResetBits(GPIOA, DATA_PIN);

        GPIO_SetBits(GPIOA, CLOCK_PIN);
        GPIO_ResetBits(GPIOA, DATA_PIN);
    }

    GPIO_ResetBits(GPIOA, CLOCK_PIN);
    GPIO_SetBits(GPIOA, LATCH_PIN);
}
