//
// Created by Jamie on 5-1-2017.
//

#ifndef MAINPROJECT_RGBDRIVER_H
#define MAINPROJECT_RGBDRIVER_H

#include <stm32f0xx_gpio.h>

#define LATCH_PIN GPIO_Pin_0
#define CLOCK_PIN GPIO_Pin_1
#define DATA_PIN GPIO_Pin_2

typedef enum {
    LEDGREEN,
    LEDRED,
    LEDYELLOW
}LEDColor_t;

namespace RGB {
    void init();
    void setFrequency(float pulse);
    void setColor(LEDColor_t color);
    void setShiftRegister(uint8_t data);
    void disable();
    void delay(const int d);
}


#endif //MAINPROJECT_RGBDRIVER_H
