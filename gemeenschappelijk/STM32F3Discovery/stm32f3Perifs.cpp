/******************************************************************************
 * Copyright      : 2010-2013 - E.J Boks
 ******************************************************************************
 Dit bestand bevat drivers voor de STM32F3 Peripherals
******************************************************************************/
/* $Id: stm32f3Perifs.cpp 2279 2016-08-29 15:00:23Z ewout $                             */

#include <stm32f30x.h>
#include "stm32f3Perifs.h"
#include "stm32f3Controller.h"

#define USART1_DR_Base  0x40013804
#define USART2_DR_Base  0x40004404


#ifdef __cplusplus
extern "C" {
#endif
    int ser_putchar (int c)
    {
        return(STM32Uart::putCh(c));

    }

    int ser_getchar (void)
    {
        return(STM32Uart::getCh());

    }



    /******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

    void USART1_IRQHandler(void)
    {
                                                        
        // Verify if this was a receive interrupt
        if(USART1->CR1 & USART_FLAG_RXNE)
        {
            // clear the interrupt
            USART1->CR1 &= ~(USART_FLAG_RXNE);
          
            /* Data inlezen en kijken of dit het einde is */
            if((STM32Uart::rx_buffer[STM32Uart::rx_cnt++] = (uint8_t)(USART1->RQR & 0x1ff))== 'q')
            {
                STM32Uart::rx_cnt=0;

                /* stuur versie naar PC applicatie */
                USART_SendData(USART1, STM32Uart::VERSION);

                STM32Uart::rx_ready = true;
            }
        }
    }

#ifdef __cplusplus
}
#endif

/* container voor regeltechniek data */
RGTData::RGTData(const int32_t mv, 
                 const int32_t rv, 
                 const int32_t cv) : measurementValue(0), //__rev(mv)),	/* draai bytes om ivm Hugo's formaat */
                                     referenceValue(0), //__rev(rv)),
                                     controlValue(0) //__rev(cv))
{

};



volatile uint8_t STM32Uart::rx_buffer[10];
volatile bool STM32Uart::rx_ready = false;
//bool STM32Uart::buffer1=true;

volatile uint32_t STM32Uart::rx_cnt = 0;


// DMA_InitTypeDef STM32Uart::DMA_InitStructure = 
// {
//     USART1_DR_Base,	/* bestemming */
//     (u32)Null,			/* bron. hier nog NULL ingevuld, later invullen */				 
//     DMA_DIR_PeripheralDST,
//     0,					 /* later zetten als lengte bekend is */
//     DMA_PeripheralInc_Disable,
//     DMA_MemoryInc_Enable,
//     DMA_PeripheralDataSize_Byte,
//     DMA_MemoryDataSize_Byte,
//     DMA_Mode_Normal,
//     DMA_Priority_High,
//     DMA_M2M_Disable
// };

STM32Uart::STM32Uart() : bufPtr((char *)txBuffer[0]),
    bufTeller(0),
    buffer1(true)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable GPIO clock */
//    RCC_AHBPeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);
  
    /* Enable USART clock */
    USARTx_CLK_INIT(USARTx_CLK, ENABLE);
  
    /* Enable the DMA clock */
 //   RCC_AHBPeriphClockCmd(USARTx_DMAx_CLK, ENABLE);
  
    /* USARTx GPIO configuration -----------------------------------------------*/ 
    /* Connect USART pins to AF7 */
//    GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);
//    GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);
  
    /* Configure USART Tx and Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
    GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
    GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
    GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);
 
    /* USARTx configuration ----------------------------------------------------*/
    /* Enable the USART OverSampling by 8 */
    USART_OverSampling8Cmd(USARTx, ENABLE); 
  
    /* USARTx configured as follow:
       - BaudRate = 5250000 baud
       - Maximum BaudRate that can be achieved when using the Oversampling by 8
       is: (USART APB Clock / 8) 
       Example: 
       - (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
       - (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
       - Maximum BaudRate that can be achieved when using the Oversampling by 16
       is: (USART APB Clock / 16) 
       Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
       Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
       - Word Length = 8 Bits

       - one Stop Bit
       - No parity
       - Hardware flow control disabled (RTS and CTS signals)
       - Receive and transmit enabled
    */ 
    USART_InitStructure.USART_BaudRate = 5250000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /* When using Parity the word length must be configured to 9 bits */
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);

    /* Configure DMA controller to manage USART TX and RX DMA request ----------*/  
    DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_DR_ADDRESS;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
/*
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
*/

   /****************** USART will Transmit Specific Command ******************/ 
    /* Prepare the DMA to transfer the transaction command (2bytes) from the
       memory to the USART */  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(Null);
    DMA_InitStructure.DMA_BufferSize = 0;
//    DMA_Init(USARTx_TX_DMA_STREAM, &DMA_InitStructure);

    /* Enable USART */
    USART_Cmd(USARTx, ENABLE);


    // GPIO_InitTypeDef       GPIO_InitStructure;
    // USART_InitTypeDef      USART_InitStruct;
    // USART_ClockInitTypeDef USART_ClockInitStruct;
    // NVIC_InitTypeDef       NVIC_InitStructure;


    /* oud uit F1 serie */
    // // --------------------------------------------------------------------------
    // // Initialize USART1
 
    // // Setup Tx pin en RTS pin
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_12;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);

    // // Setup Rx pin en CTS pin
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    // GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART_ClockStructInit(&USART_ClockInitStruct);
    // USART_ClockInit(USART1, &USART_ClockInitStruct);

    // // Set defaults
    // USART_StructInit(&USART_InitStruct);
    // USART_InitStruct.USART_BaudRate = 921600; // Override baudrate
    // USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_RTS_CTS; /* met HW stroomcontrole */

    // USART_Init(USART1, &USART_InitStruct);
  
    // // Enable the USART1 Interrupt
    // NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    // NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    // NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    // NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    // NVIC_Init(&NVIC_InitStructure);

    // // Enable RX interrupt
    // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);


    // /* stel in DMA voor verzenden naar de Desktop PC */
    // USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
  
    // /* DMA1 Channel4 (triggered by USART1 Tx event) Config */
    // DMA_DeInit(DMA1_Channel4);

    // USART_Cmd(USART1, ENABLE);          

}

    
void STM32Uart::putStr (const char *p)
{



}

void STM32Uart::getStr (const unsigned short l,  
                        char *p)
{



}

int STM32Uart::putCh(const char ch,FILE *file)
{
    USART_SendData(USARTx,ch);
    return(ch);
}

int STM32Uart::getCh (FILE *file)
{


    return(0);
}

char STM32Uart::wachtOpDesktopCommando()const
{
    // Wait for commands from PC application
    do
    {
			//__wfi();  /* ga slapen tot UART een interrupt verzorgt */
    } while(rx_ready == false);

    rx_ready = false;

    return(rx_buffer[0]);
}

// ----------------------------------------------------------------------------
// Transmit the title, always 30 characters
// ----------------------------------------------------------------------------
void STM32Uart::transmitTitle(const char * const s)
{
    assert(txBufferSize >= 30);

    uint32_t i=0;
  		
    // Transmit the string
    while((i<29) &&  (*(s+i)!='\0') && (*(s+i)!='\n'))
    {
        /* vul de buffer */
        *(bufPtr+i) = *(s+i);
        i++;
    }

    // Transmit white spaces until 30 characters in total
    while(i<29)
    {
        *(bufPtr+i) = (char)(' ');
        i++;
    }

    /* forceer een newline op de laatste plaats */
    *(bufPtr+29) = '\n';

    bufTeller=30;

    dmaVerzendNu();

}


void STM32Uart::transmitData(const RGTData &rgtData)                           
{  
    if ( bufTeller == txBufferSize)
        dmaVerzendNu();
		
    memcpy(bufPtr,(unsigned char *)&rgtData,sizeof(RGTData));
    bufPtr+=sizeof(RGTData);
    bufTeller+=sizeof(RGTData);

}

void STM32Uart::dmaVerzendNu()
{

//    while (0 != DMA_GetCurrDataCounter(USARTx_TX_DMA_STREAM));
//
//    DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
//
//    /* DMA1 Channel4 (triggered by USARTx Tx event) Config */
//    DMA_InitStructure.DMA_BufferSize = bufTeller;
//    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)txBuffer[buffer1];
//    DMA_Init(USARTx_TX_DMA_STREAM, &DMA_InitStructure);
//
//    /* Verstuur de huidige buffer */
//    DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);

    /* wijzig buffers */
    buffer1 = ((buffer1==true) ? false : true);
    bufPtr =  (char *)txBuffer[buffer1]; 
    bufTeller=0;

}

STM32DAC::STM32DAC(const int16_t startWaarde)
{
  

    DAC_DeInit();
 
    // --------------------------------------------------------------------------
    // Initialize DAC
    // Configure PA.05 (DAC) as output
    GPIO_InitTypeDef GPIO_InitStructure = 
        {
            GPIO_Pin_5,
            GPIO_Mode_AN,
            GPIO_Speed_2MHz,
            GPIO_OType_PP,
            GPIO_PuPd_NOPULL
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
  

    DAC_Init(DAC_Channel_1, &DAC_InitStructure); 

    DAC_Cmd(DAC_Channel_1, ENABLE); // DAC Signaal
 
}

void STM32DAC::set(const uint16_t value) const
{
    const uint16_t signaal  = ((value>4095) ? 4095 : value);	  /* 12 bits */

    DAC_SetChannel2Data(DAC_Align_12b_R, signaal); // DAC Signaal Fuzzy

}

#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)

STM32ADC::STM32ADC(const unsigned short st) : timer(TIM2),sampTijd(st)
{
  /*  ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef       DMA_InitStructure;*/
    GPIO_InitTypeDef      GPIO_InitStructure;

    /* Enable ADC3, DMA2 and GPIO clocks ****************************************/
 //   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

    /* DMA2 Stream0 channel2 configuration **************************************/
 /*   DMA_InitStructure.DMA_Channel = DMA_Channel_2;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&conversieWaarde;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = 1;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream0, ENABLE);
*/
    /* Configure ADC3 Channel7 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    /* ADC Common Init **********************************************************/
 /*   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);
*/
    /* ADC3 Init ****************************************************************/
 /*   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC3, &ADC_InitStructure);

  */  /* ADC3 regular channel7 configuration *************************************/
//    ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);

    /* Enable DMA request after last transfer (Single-ADC mode) */
  //  ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);

    /* Enable ADC3 */
    ADC_Cmd(ADC3, ENABLE);
 
    // /* timer clock enable */
    // RCC_ADCCLKConfig(RCC_PCLK2_Div2);


    // GPIO_InitTypeDef GPIO_InitStructure = {  GPIO_Pin_5,
    //                                          GPIO_Mode_AN,
    //                                          GPIO_Speed_2MHz,
    //                                          GPIO_OType_PP,
    //                                          GPIO_PuPd_NOPULL };

    // GPIO_Init(GPIOA, &GPIO_InitStructure);
  

    // // --------------------------------------------------------------------------
    // // Initialize ADC
   
    // ADC_DeInit(ADC1);

    // // ADC1 Configuration
    // ADC_InitTypeDef ADC_InitStructure = {ADC_Mode_Independent,
    //                                      DISABLE,
    //                                      DISABLE,
    //                                      ADC_ExternalTrigConv_T2_CC2,
    //                                      ADC_DataAlign_Right,
    //                                      1 };

    // ADC_Init(ADC1, 
    //          &ADC_InitStructure);

    // ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5); // ADC Fuzzy 
    // /* Enable ADC1 external trigger conversion */ 
    // ADC_ExternalTrigConvCmd(ADC1, ENABLE);

    // /* zet de sampling timer T1 op */ 
    // /* TIM2 clock enable */
    // RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    // zetSampTijd(sampTijd);

    // // Enable ADC
    // ADC_Cmd(ADC1, ENABLE);

    /* timer counter disable */
    TIM_Cmd(timer, DISABLE);

}

uint16_t STM32ADC::readAnalog() const
{
    /* let op: is de conversie gestart met startStopBemonstering() ? */ 
    // Wait until conversion completion
    while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET){;}
  
    // Get the conversion value
    return(ADC_GetConversionValue(ADC3));

}


void STM32ADC::startStopBemonstering(const bool stand)
{
    const FunctionalState staat = ((true == stand) ? ENABLE : DISABLE);

    TIM_Cmd(timer,staat);
}

void STM32ADC::zetSampTijd(const unsigned short ts)	 /* in millisecs */
{
    assert(ts<65535);

    const unsigned int ts_micro = ts*1000;

    zetSampTijd(ts_micro);
}

void STM32ADC::zetSampTijd(const unsigned int ts)  /* in microsecs */
{
    assert(ts<65535);
    sampTijd = ts;

    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_OCInitTypeDef         TIM_OCInitStructure;

    /* timer configuration ------------------------------------------------------*/ 
    /* Time Base configuration */
    /* systeemklok voor perifs = 48 MHz */
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = sampTijd;          
    TIM_TimeBaseStructure.TIM_Prescaler = 48;        /* klok tikt in microsecs */      
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(timer, &TIM_TimeBaseStructure);

    /* timer channel1 configuration in Output Compare mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = sampTijd; 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;         
    TIM_OC2Init(timer, &TIM_OCInitStructure);

    /* timer main Output Enable */
    TIM_CtrlPWMOutputs(timer, ENABLE);  
}



/********* I2C Driver ************/
STM32I2C::STM32I2C(I2C_TypeDef * const device) : i2c(device)
{

    GPIO_InitTypeDef      GPIO_InitStructure;
    DMA_InitTypeDef       DMA_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
 //   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1+RCC_AHB1Periph_GPIOB, ENABLE);

    I2C_DeInit(i2c);

//    GPIO_PinAFConfig(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_SOURCE, I2Cx_SDA_AF);
//    GPIO_PinAFConfig(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_SOURCE, I2Cx_SCL_AF);
  
    /* Configure I2C Tx and Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
    GPIO_InitStructure.GPIO_Pin = I2Cx_SDA_PIN;
    GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = I2Cx_SCL_PIN;
    GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
 
 
    I2C_InitTypeDef I2C_InitStruct = 
        {
            (100*1000),
            I2C_Mode_I2C,
            50,
            0x0,  
            0,
            0
        };

    I2C_Init(i2c,&I2C_InitStruct);

    /* DMA1 Stream6 channel1 configuration : TX uitvoering ******************/
//    DMA_InitStructure.DMA_Channel = DMA_Channel_1;
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(i2c->DR);
//    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Null;
//    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//    DMA_InitStructure.DMA_BufferSize = 1;
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//    DMA_Init(DMA1_Stream6, &DMA_InitStructure);
//    DMA_Cmd(DMA1_Stream6, ENABLE);

    memcpy((UInt8 *)&txDef,
           (UInt8 *)&DMA_InitStructure,
           sizeof(DMA_InitTypeDef));



    /* DMA1 Stream5 channel1 configuration : RX uitvoering ******************/
/*    DMA_InitStructure.DMA_Channel = DMA_Channel_1;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);*/
//    DMA_Cmd(DMA1_Stream5, ENABLE);

    memcpy((UInt8 *)&rxDef,
           (UInt8 *)&DMA_InitStructure,
           sizeof(DMA_InitTypeDef));

}

void STM32I2C::verstuur(const UInt8 adr,   
                        const UInt8 dAdr,   
                        UInt8 const * const data,
                        const Teller lengte)
{
/*    txDef.DMA_Memory0BaseAddr = (uint32_t)data;
    txDef.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    txDef.DMA_BufferSize=lengte;

    DMA_Init(DMA1_Stream6, &txDef);
    DMA_Cmd(DMA1_Stream6, ENABLE);*/

}
