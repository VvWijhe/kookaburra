//
// Created by Jamie on 9-10-2016.
//

#include "usart.h"

void USART_1::init() {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // Initialize USART1
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    // Setup Tx- and Rx pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART1_IRQn);
}

void USART_1::operator<<(const char *str) {
    puts(str);
}

void USART_1::operator<(char c) {
    /// TODO: replace cout with usart_putc
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { ; }
    USART_SendData(USART1, (uint16_t) c);
}

void USART_1::operator<<(uint32_t number) {
    char buffer[30];

    itoa(number, buffer, 10);
    puts(buffer);
}

void USART_1::puts(const char *str) {
    while (*str) {
        if (*str == '\n') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { ; }
            USART_SendData(USART1, '\r');
        }

        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { ; }
        USART_SendData(USART1, *str++);
    }
}

void USART_1::operator>(char &c) {
    //cin >> c;
}

void USART_1::operator>>(char c[]) {
    //fflush(stdin);
    //cin.get(c, 10);
}

void USART_1::Clearscreen(void)
{
    char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
    char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home

    puts(cmd1);
    puts(cmd2);
}

