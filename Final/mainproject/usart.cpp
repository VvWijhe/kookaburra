//
// Created by Jamie on 9-10-2016.
//

#include "usart.h"

uint8_t rxBuffer[10] = "";
uint8_t indexBuffer = 0;

UART uart;

UART::UART():isInitialized(false){

}

void UART::init() {
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

    isInitialized = true;
}

UART &UART::operator<<(const char *str) {
    assert_param(isInitialized);

    puts(str);
    return *this;
}

UART &UART::operator<<(uint32_t number) {
    assert_param(isInitialized);

    char buffer[30];

    itoa((int) number, buffer, 10);
    puts(buffer);
    return *this;
}

//UART &UART::operator<<(bool boolean) {
//    boolean ? this->puts("True") : this->puts("false");
//    return *this;
//}

void UART::puts(const char *str) {
    assert_param(isInitialized);
    while (*str) {
        if (*str == '\n') {
            while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { ; }
            USART_SendData(USART1, '\r');
        }

        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) { ; }
        USART_SendData(USART1, (uint16_t) *str++);
    }
}

void UART::operator>>(char c[]) {
    assert_param(isInitialized);
    //fflush(stdin);
    //cin.get(c, 10);
}

void UART::Clearscreen() {
    assert_param(isInitialized);

    char clr[] = {0x1B, '[', '2', 'J', '\0'};
    char crsh[] = {0x1B, '[', 'f', '\0'};

    puts(clr);
    puts(crsh);
}

