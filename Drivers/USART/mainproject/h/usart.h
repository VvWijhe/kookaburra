//
// Created by Jamie on 9-10-2016.
//
#include <stdlib.h>
#include <string.h>
#include <cstdio>

#include "stm32f0xx_usart.h"

#ifndef USART_H
#define USART_H

class USART_1 {
public:
    void init();

    void operator<<(const char *str);

    void operator<(char c);

    void operator<<(uint32_t number);

    void puts(const char *str);

    void operator>(char &c);

    void operator>>(char c[]);

private:
};

#endif //USART_H
