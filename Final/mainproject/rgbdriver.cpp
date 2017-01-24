//
// Created by Jamie on 5-1-2017.
//

#include <stm32f0xx_gpio.h>
#include "rgbdriver.h"

void RGB::init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);

    // Init shift register gpio
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Pin = LATCH_PIN | CLOCK_PIN | DATA_PIN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // Just in case, set all bits low
    GPIO_ResetBits(GPIOC, CLOCK_PIN);
    GPIO_ResetBits(GPIOC, DATA_PIN);
    GPIO_ResetBits(GPIOC, LATCH_PIN);
    disable();
}

void RGB::setFrequency(float pulse) {
    if(pulse > 0.01 && pulse < 30) {
        //Enable clock for the timer
        RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
        // Enable ARR preloading
        TIM17->CR1 |= TIM_CR1_ARPE;
        // Enable CCR1 preloading
        TIM17->CCMR1 |= TIM_CCMR1_OC2PE;
        // PWM mode 1: OC1M = 110
        TIM17->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
        // Prescaler keeps zero to allow control PWM frequency
        TIM17->PSC = (uint16_t) (20000 - 1);
        // PWM Frequency 36 kHz
        TIM17->ARR = (uint32_t) (((1 / pulse) * 400) - 1);
        // Duty cycle: 33%
        TIM17->CCR1 = (uint32_t) (((1 / pulse) * 400) * 0.9);
        // Uncomment to change polarity
        //TIM2->CCER |= TIM_CCER_CC2P;
        // Enable Capture/Compare output 1, ie PB9
        TIM17->CCER |= TIM_CCER_CC1E;
        // Main Output enable
        TIM17->BDTR |= TIM_BDTR_MOE;
        // Start timer
        TIM17->CR1 |= TIM_CR1_CEN;
    }
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
    // Set all bits low
    setShiftRegister(0);
}

void RGB::setShiftRegister(uint8_t data) {
    GPIO_ResetBits(GPIOC, LATCH_PIN);

    // Shift data and send to shift register
    for (int i = 0; i < 8; i++) {
        if ((data & 0b10000000) == 0b10000000) {
            GPIO_SetBits(GPIOC, DATA_PIN);
        } else {
            GPIO_ResetBits(GPIOC, DATA_PIN);
        }

        // Clock the register
        GPIO_SetBits(GPIOC, CLOCK_PIN);
        delay(SystemCoreClock / 8 / 500);
        GPIO_ResetBits(GPIOC, CLOCK_PIN);

        data <<= 1;
    }

    // Pull latch pin high to set the output
    GPIO_ResetBits(GPIOC, DATA_PIN);
    delay(SystemCoreClock / 8 / 500);
    GPIO_SetBits(GPIOC, LATCH_PIN);
}

void RGB::delay(const int d) {
    volatile int i;

    for (i = d; i > 0; i--) { ; }

    return;
}
