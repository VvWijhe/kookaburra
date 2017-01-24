//
// Created by Jamie on 9-10-2016.
//
#include <stdlib.h>
#include <string.h>
#include <cstdio>


#include "stm32f0xx_usart.h"

#ifndef USART_H
#define USART_H

extern uint8_t rxBuffer[10];
extern uint8_t indexBuffer;

/**
 * Description:
 * USART driver for the stm32. USART1 and USART2 are classes that contain
 * functions for sending and reading to and from a device. The send function
 * automatically recognizes a string or a number.
 *
 * Usage:
 * (1)Edit the init function to initialise the usart to your goals.
 * (2)Declare a USART1 or USART2 class as a global or local variable.
 * (3)Use the << operator to transmit a string. Use the < operator to transmit a character.
 * (4a)If using polling: Use >> to receive a string. Use > to receive a character. Use a buffer to store the data.
 * (4b)If using interrupts, add the 'void USART1_IRQHandler()' prototype in the main.h file under the c++ macro.
 * define this function in the main.cpp file. The USART_ReceiveData must be called in this function to clear the
 * RXNE flag.
 */

class UART {
public:
    UART();

    void init();

    UART &operator<<(const char *str);

    UART &operator<<(uint32_t number);

    UART &operator<<(bool boolean);

    void puts(const char *str);

    void operator>>(char c[]);

    void Clearscreen();

private:
    bool isInitialized;
};

extern UART uart;


#endif //USART_H
