/******************************************************************************
 * Project        : ES2 Project 3
 * Copyright      : 2010-2013 - H. Arends en E.J Boks
 ******************************************************************************
 Dit bestand bevat drivers voor de STM32 Peripherals
******************************************************************************/
/* $Id: stm32Periphs.cpp 2038 2016-01-05 16:20:54Z ewout $                             */

#include "stm32Periphs.h"

#include <stm32f10x_tim.h>

#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

int ser_putchar(int c) {
    USART_putchar(USART1, c);
    return (c);

}

int ser_getchar(void) {
    return (USART_getchar(USART1));
}

#ifdef __cplusplus
}
#endif

STM32Uart::STM32Uart() {

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStruct;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA + RCC_APB2Periph_USART1), ENABLE);

    // --------------------------------------------------------------------------
    // Initialize USART1

    // Setup Tx pin en RTS pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Setup Rx pin en CTS pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_ClockStructInit(&USART_ClockInitStruct);
    USART_ClockInit(USART1, &USART_ClockInitStruct);

    // Set defaults
    USART_StructInit(&USART_InitStruct);
    USART_InitStruct.USART_BaudRate = 921600; // Override baudrate
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS; /* met HW stroomcontrole */

    USART_Init(USART1, &USART_InitStruct);

    // Enable the USART1 Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Enable RX interrupt
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART1, ENABLE);

}


int STM32Uart::putCh(const char ch, FILE *file) {
    USART_putchar(USART1, ch);
    return (ch);
}

int STM32Uart::getCh(FILE *file) {


    return (0);
}


STM32DAC::STM32DAC(const STM32DAC::DACKeuze keuze) : dacKeuze(keuze) {

    assert(keuze < DACAAantalKeuzen);

    RCC_APB1PeriphClockCmd((RCC_APB1Periph_DAC), ENABLE);
    DAC_DeInit();

    // --------------------------------------------------------------------------
    // Initialize DAC
    // Configure PA.05 (DAC) as output
    GPIO_InitTypeDef GPIO_InitStructure =
            {
                    ((keuze == DAC2Keuze) ? GPIO_Pin_5 : GPIO_Pin_4),
                    GPIO_Speed_2MHz,
                    GPIO_Mode_Out_PP
            };

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // DAC Channel2 Configuration
    DAC_InitTypeDef DAC_InitStructure =
            {
                    DAC_Trigger_None,
                    DAC_WaveGeneration_None,
                    0,
                    DAC_OutputBuffer_Enable
            };


    DAC_Init(((keuze == DAC2Keuze) ? DAC_Channel_2 : DAC_Channel_1),
             &DAC_InitStructure);

    DAC_Cmd(((keuze == DAC2Keuze) ? DAC_Channel_2 : DAC_Channel_1), ENABLE); // DAC Signaal

}

void STM32DAC::set(const UInt16 value) const {
    const UInt16 signaal = ((value > 4095) ? 4095 : value);      /* 12 bits */

    if ((dacKeuze == DAC2Keuze))
        DAC_SetChannel2Data(DAC_Align_12b_R, signaal); // DAC Signaal Fuzzy
    else
        DAC_SetChannel1Data(DAC_Align_12b_R, signaal); // DAC Signaal
}

STM32ADC::STM32ADC(const STM32ADC::ADCKeuze keuze) : timer(TIM2),
                                                     timBus(RCC_APB1Periph_TIM2),
                                                     adcTimTrigger(ADC_ExternalTrigConv_T2_CC2),
                                                     adcKeuze(keuze) {

    assert(keuze < ADC1AantalKanalen);

    RCC_APB2PeriphClockCmd((RCC_APB2Periph_ADC1), ENABLE);

    static const UInt16 adcgpio[ADC1AantalKanalen] =
            {
                    GPIO_Pin_1,
                    GPIO_Pin_2,
                    GPIO_Pin_3,
                    GPIO_Pin_6
            };

    GPIO_InitTypeDef GPIO_InitStructure = {adcgpio[keuze],
                                           GPIO_Speed_50MHz,
                                           GPIO_Mode_AIN};

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    static const UInt8 adckanalen[ADC1AantalKanalen] =
            {
                    ADC_Channel_1,
                    ADC_Channel_2,
                    ADC_Channel_3,
                    ADC_Channel_6
            };

    const uint8_t kanaal = adckanalen[keuze];

    /* ADC1 clock enable */
    RCC_ADCCLKConfig(RCC_PCLK2_Div2);

    ADC_DeInit(ADC1);

    // ADC1 Configuration
    ADC_InitTypeDef ADC_InitStructure = {ADC_Mode_Independent,
                                         DISABLE,
                                         DISABLE,
                                         adcTimTrigger,
                                         ADC_DataAlign_Right,
                                         1};

    ADC_Init(ADC1,
             &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, kanaal, 1, ADC_SampleTime_239Cycles5);
    /* Enable ADC1 external trigger conversion */
    ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    /* zet de sampling timer T1 APB klok aan maar schakel de timer uit */
    RCC_APB1PeriphClockCmd(timBus, ENABLE);
    TIM_Cmd(timer, DISABLE);

    // Enable ADC
    ADC_Cmd(ADC1, ENABLE);


    /// Enable ADC1 reset calibaration register
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1)) { ; }
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1)) { ; }

}

UInt16 STM32ADC::readAnalog() const {
    /* let op: is de conversie gestart met startStopBemonstering() ? */
    // Wait until conversion completion
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) { ; }

    // Get the conversion value
    return (ADC_GetConversionValue(ADC1));

}


void STM32ADC::startStopBemonstering(const bool stand) {
    const FunctionalState staat = ((true == stand) ? ENABLE : DISABLE);

    TIM_Cmd(timer, staat);
}

void STM32ADC::zetSampTijd(const UInt16 ts)     /* in millisecs */
{
    assert(ts < 65535);

    const UInt32 ts_micro = ts * 1000;

    zetSampTijd(ts_micro);
}

void STM32ADC::zetSampTijd(const UInt32 ts)  /* in microsecs */
{
    assert(ts < 65535);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    /* timer configuration ------------------------------------------------------*/
    /* Time Base configuration */
    /* systeemklok voor perifs = PLL/2 = 12 MHz */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = ts;
    TIM_TimeBaseStructure.TIM_Prescaler = 24;        /* klok tikt in microsecs */
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(timer, &TIM_TimeBaseStructure);

    /* timer channel1 configuration in Output Compare mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = ts;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC2Init(timer, &TIM_OCInitStructure);

}


