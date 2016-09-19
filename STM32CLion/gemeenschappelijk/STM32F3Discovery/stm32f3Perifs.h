/******************************************************************************
 * Project        : ES3 DSB Practicum
 * Copyright      : 2010-2013 - E.J Boks
 ******************************************************************************
 Dit bestand bevat de API voor de STM32F3 Peripherals
******************************************************************************/
/* $Id: stm32f3Perifs.h 2279 2016-08-29 15:00:23Z ewout $                             */

#ifndef HANESE_STM32F3PERIFS_H_
#define HANESE_STM32F3PERIFS_H_

#include <algdef.h>

// Uncomment the line below to enable peripheral header file inclusion
   #include <stm32f30x_adc.h>
// #include <stm32f30x_bkp.h>
// #include <stm32f30x_can.h>
// #include <stm32f30x_cec.h>
// #include <stm32f30x_crc.h>
   #include <stm32f30x_dac.h>
// #include <stm32f30x_dbgmcu.h>
   #include <stm32f30x_dma.h>
   #include <stm32f30x_exti.h>
// #include <stm32f30x_flash.h>
   #include <stm32f30x_gpio.h>
   #include <stm32f30x_i2c.h>
// #include <stm32f30x_iwdg.h>
// #include <stm32f30x_pwr.h>
   #include <stm32f30x_rcc.h>
// #include <stm32f30x_rtc.h>
// #include <stm32f30x_sdio.h>
// #include <stm32f30x_spi.h>
// #include <stm32f30x_tim.h>
   #include <stm32f30x_usart.h>
// #include <stm32f30x_wwdg.h>
//   #include <misc.h>

#include <stm32f30x.h> 
#include <stdio.h>

//inline int32_t revBytes(const int32_t waarde)
//{
//	int32_t resultaat;
//	__asm
//	{
//		REV resultaat,waarde;
//	}
//	return(resultaat);
//}

/* Definition for USARTx resources ********************************************/
#define USARTx                           USART3
#define USARTx_CLK                       RCC_APB1Periph_USART3
#define USARTx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define USARTx_IRQn                      USART3_IRQn
#define USARTx_IRQHandler                USART3_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_10                
#define USARTx_TX_GPIO_PORT              GPIOC                       
#define USARTx_TX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USARTx_TX_SOURCE                 GPIO_PinSource10
#define USARTx_TX_AF                     GPIO_AF_USART3

#define USARTx_RX_PIN                    GPIO_Pin_11                
#define USARTx_RX_GPIO_PORT              GPIOC                    
#define USARTx_RX_GPIO_CLK               RCC_AHB1Periph_GPIOC
#define USARTx_RX_SOURCE                 GPIO_PinSource11
#define USARTx_RX_AF                     GPIO_AF_USART3


#define I2Cx_SDA_PIN                    GPIO_Pin_7                
#define I2Cx_SDA_GPIO_PORT              GPIOB                       
#define I2Cx_SDA_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_SOURCE                 GPIO_PinSource7
#define I2Cx_SDA_AF                     GPIO_AF_I2C1

#define I2Cx_SCL_PIN                    GPIO_Pin_6                
#define I2Cx_SCL_GPIO_PORT              GPIOB               
#define I2Cx_SCL_GPIO_CLK               RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_SOURCE                 GPIO_PinSource6
#define I2Cx_SCL_AF                     GPIO_AF_I2C1

/* Definition for DMAx resources **********************************************/
#define USARTx_DR_ADDRESS                ((uint32_t)USART3 + 0x04) 

#define USARTx_DMA                       DMA1
#define USARTx_DMAx_CLK                  RCC_AHB1Periph_DMA1
   
#define USARTx_TX_DMA_CHANNEL            DMA_Channel_4
#define USARTx_TX_DMA_STREAM             DMA1_Stream3
#define USARTx_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF3
#define USARTx_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF3
#define USARTx_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF3
#define USARTx_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF3
#define USARTx_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF3
            
#define USARTx_RX_DMA_CHANNEL            DMA_Channel_4
#define USARTx_RX_DMA_STREAM             DMA1_Stream1
#define USARTx_RX_DMA_FLAG_FEIF          DMA_FLAG_FEIF1
#define USARTx_RX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF1
#define USARTx_RX_DMA_FLAG_TEIF          DMA_FLAG_TEIF1
#define USARTx_RX_DMA_FLAG_HTIF          DMA_FLAG_HTIF1
#define USARTx_RX_DMA_FLAG_TCIF          DMA_FLAG_TCIF1

#define USARTx_DMA_TX_IRQn               DMA1_Stream3_IRQn
#define USARTx_DMA_RX_IRQn               DMA1_Stream1_IRQn
#define USARTx_DMA_TX_IRQHandler         DMA1_Stream3_IRQHandler
#define USARTx_DMA_RX_IRQHandler         DMA1_Stream1_IRQHandler   


/******************************************************************************
 Klasse definitie
******************************************************************************/
class RGTData
{
public:
	RGTData(const int32_t mv,   /* meetWaarde */
			const int32_t rv, 	/* regelwaarde */
			const int32_t cv);	/* controle waarde */

private:
	int32_t measurementValue;
	int32_t referenceValue;
	int32_t controlValue;

};

class STM32Uart
{
public: 
    STM32Uart(void);


    char wachtOpDesktopCommando() const;

    void transmitTitle(const char * const);

    void transmitData(const RGTData &); /* voor RGT practicum */
    void dmaVerzendNu();  /* verzend nog in buffer aanwezig DMA data */

    /* leden zijn static, om te kunnen koppelen aan C std library
     *  Dit is niet standaard in C++ applicaties */
    static void putStr (const char *p);  /* verstuur een string */
    static void getStr (const unsigned short l,   /* ontvang een string met lengte l */
                        char *p);

    static int putCh(const char c,
                     FILE *file=NULL);

    static int getCh (FILE *file=NULL);

    // USART variables
    static volatile uint8_t rx_buffer[10];
    static volatile bool rx_ready;
    static volatile uint32_t rx_cnt;

    static const char VERSION='2';		 /* komt van Hugo af */

protected:

	
private:
    DMA_InitTypeDef DMA_InitStructure;  

    unsigned int bufIndex;
    static const unsigned int txBufferSize=10*sizeof(RGTData);


    char *bufPtr;
    char txBuffer[2][txBufferSize];
    uint16_t bufTeller;

    bool buffer1;
};


class STM32DAC
{
public:

    STM32DAC(const int16_t startWaarde=0);

    void set(const uint16_t value) const;

protected:

private:
  
};


class STM32ADC
{
public:

    STM32ADC(const unsigned short st=5);	     /* sampling tijd in millisecs */

    void startStopBemonstering(const bool);

    void zetSampTijd(const unsigned short st);   /* sampling tijd in millisecs */
    void zetSampTijd(const unsigned int st);     /* sampling tijd in microsecs */


    uint16_t readAnalog() const;

protected:

private:

    uint16_t conversieWaarde;

    TIM_TypeDef * const timer;
    unsigned short sampTijd;       //* sampling tijd in millisecs */
};



class STM32I2C
{
public:
    STM32I2C(I2C_TypeDef * const);   /* I2C perif nummer : 0 -2 */

    void verstuur(const UInt8,           /* busadres */
                  const UInt8,           /* device adres */ 
                  UInt8 const * const,   /* data */
                  const Teller);         /* lengte van de data */

private:

    I2C_TypeDef * const i2c; 
    DMA_InitTypeDef txDef,rxDef;
};


#endif // Perifs
 
