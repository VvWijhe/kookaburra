/**
  ******************************************************************************
  * @file    cpalDriver.c
  * @author  Ewout Boks
  * @version V1.1.1
  * @date    15-12-2015
  * @brief   Aangepaste driver voor de STM32F0
  *            
  *                 
  * $Id: stm32f0basis.cpp 2039 2016-01-05 16:26:34Z ewout $ */

#include <assert.h>
#include <string.h>
#include <stm32f0basis.h>
#include <cpaldriver.h>
#include <stm32f0xx.h>

typedef unsigned int Teller;
typedef unsigned int UInt32;

void wachtklokInit(void) {
    /* zet de systick op 1 millisec interval */
    SysTick_Config((SystemCoreClock / 1000));

    NVIC_SetPriority(SysTick_IRQn, 0);
}


/* volatile omdat anders met -Os compilatie slechts een register en geen mem lokatie wordt geevalueerd. */
static volatile Teller wachtTikken = 0;

void wachtMilliSecs(const uint32_t msecs) {

    wachtTikken = msecs;

    while (0 != wachtTikken);
}

/*! @note let op !!! een interrupt service handler heeft C-linkage nodig, anders is de koppeling 
 * met de vector tabel ongeldig (C++ name mangling) */
#ifdef __cplusplus
extern "C" {
#endif

/* De systick interrupt handler */
void SysTick_Handler(void) {
    if (0 != wachtTikken)
        wachtTikken--;

    CPAL_I2C_TIMEOUT_Manager();
}

#ifdef __cplusplus
}
#endif


LEDDriver::LEDDriver() {
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

const uint16_t LEDDriver::pinSources[LEDDriver::LEDAantalLEDs] = {GPIO_Pin_8, GPIO_Pin_9};

void LEDDriver::schakel(const LEDid id,
                        const bool stand) {
    assert(id < LEDAantalLEDs);

    GPIO_WriteBit(GPIOC, pinSources[id], (false == stand) ? Bit_RESET : Bit_SET);
}

void LEDDriver::om(const LEDid id) {
    const BitAction huidig = static_cast<BitAction>(GPIO_ReadOutputDataBit(GPIOC, pinSources[id]));

    schakel(id, ((Bit_RESET == huidig) ? true : false));
}

void USARTinit(void) {

    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

    //Configure pins:  Rx and Tx --------------------------
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

static char *zendData;
static uint32_t zendLengte, zendKlaar;

void USARTsend(char const *stringPtr) {
    assert(0 != stringPtr);
    zendData = (char *) (stringPtr);
    zendLengte = strlen(stringPtr);
    zendKlaar = 0;

    USART_ITConfig(USART1, USART_IT_TXE, ENABLE); /* dit is de interrupt waarop we gaan werken */
    while (0 == zendKlaar); /* wacht hier */
    USART_ITConfig(USART1, USART_IT_TXE, DISABLE); /* zet weer uit */

}

#ifdef __cplusplus
extern "C" {
#endif

void USART1_IRQHandler(void) {
    if (SET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) {
        USART_SendData(USART1, *(zendData++));

        if (0 == (--zendLengte))
            zendKlaar = 1;
    }
}

#ifdef __cplusplus
}
#endif
