/******************************************************************************
 * Project        : ES2 Project 3 en DSB Practicum
 * Copyright      : 2010-2015 - H. Arends en E.J Boks
 ******************************************************************************
 Dit bestand bevat de API voor de STM32 Peripherals
******************************************************************************/
/* $Id: stm32Periphs.h 2038 2016-01-05 16:20:54Z ewout $                             */

#ifndef HANESE_STM32PERIFS_H_
#define HANESE_STM32PERIFS_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <algdef.h>

// Uncomment the line below to enable peripheral header file inclusion
   #include "stm32f10x_adc.h"
// #include "stm32f10x_bkp.h"
// #include "stm32f10x_can.h"
// #include "stm32f10x_cec.h"
// #include "stm32f10x_crc.h"
   #include "stm32f10x_dac.h"
// #include "stm32f10x_dbgmcu.h"
   #include "stm32f10x_dma.h"
   #include "stm32f10x_exti.h"
// #include "stm32f10x_flash.h"
// #include "stm32f10x_fsmc.h"
   #include "stm32f10x_gpio.h"
// #include "stm32f10x_i2c.h"
// #include "stm32f10x_iwdg.h"
// #include "stm32f10x_pwr.h"
   #include "stm32f10x_rcc.h"
// #include "stm32f10x_rtc.h"
// #include "stm32f10x_sdio.h"
// #include "stm32f10x_spi.h"
// #include "stm32f10x_tim.h"
   #include "stm32f10x_usart.h"
// #include "stm32f10x_wwdg.h"
   #include "misc.h"


/******************************************************************************
  Defines
******************************************************************************/
#define CR        ('\r')
#define LF        ('\n')
#define BACKSPACE ('\b')
#define SPACE     (' ')
#define ESC       (0x1B)



/******************************************************************************
 Klasse definitie
******************************************************************************/


template<typename ttype>
class FixedDataPakket
{
	public:
	
  /*! @brief constructor voor static allocatie */	
	FixedDataPakket()  : data((ttype *)NULL),grootte(0)
	{
		
	};
	
	FixedDataPakket(ttype const * const dataPtr,
						      const UInt32 g) : data(dataPtr),grootte(g)
		{
			assert(NULL != dataPtr);
			assert(0 != grootte);
		};
		
	UInt32 aantal() const
	{
		return(grootte);
	}
	
	ttype & operator [] (const UInt32 index)
	{
		assert(index < grootte);
		return(*(data+index));
	};
	
	protected:
		ttype *data;
	  UInt32 grootte;
};

template<typename ttype, const UInt32 Grootte>
class DataPakket
{
	public:
		
	DataPakket()
	{
			memset(data,0,sizeof(ttype)*Grootte);
	};
		
	DataPakket(ttype const * const dataPtr) 
		{
			assert(NULL != dataPtr);
			(*this) = dataPtr;
		};
		
	UInt32 aantal() const
	{
		return(Grootte);
	};
	
	DataPakket<ttype,Grootte> & operator =  (ttype const * const dataPtr) 
	{
		memcpy((UInt8 *)data,(UInt8 *)dataPtr,sizeof(ttype)*Grootte);
		return(*this);
	}
	
	DataPakket<ttype,Grootte> & operator = (const DataPakket<ttype,Grootte> &rhs)
	{
		return((*this) = rhs.data);
	};
	
	ttype & operator [] (const UInt32 index)
	{
		assert(index < Grootte);
		return(data[index]);
	};
	
	protected:
		ttype data[Grootte];
};

template<typename ttype, const UInt32 Grootte>
class VerzendOntvangBuffer : public DataPakket<ttype,Grootte>	
{
public:
	VerzendOntvangBuffer() : DataPakket<ttype,Grootte>(),count(0)
	{
	
	};
	
	VerzendOntvangBuffer(ttype const * const dataPtr) : DataPakket<ttype,Grootte>(dataPtr),count(0)
	{
	
	};

	VerzendOntvangBuffer(const VerzendOntvangBuffer<ttype,Grootte> &rhs)
	{
		DataPakket<ttype,Grootte>::operator = (rhs);
		count = rhs.count;
	}
	
	void reset()
	{
		count=0;
	};
	
	UInt32 geefIndex() const
	{
		return(count);
	};
	
	bool klaar() const
	{
		return(count==Grootte);
	};
	
	ttype zend(const bool verhoog=true)
	{
		const ttype veld = DataPakket<ttype,Grootte>::data[count];
		if (true == verhoog)
			count++;
		return(veld);
	};
	
	bool ontvang(const ttype val,const bool verhoog=true)
	{
		DataPakket<ttype,Grootte>::data[count] = val;
				
			if (true == verhoog)
				count++;
			
			return(klaar());
	};
	
	/*! @brief forceer dat de buffer klaar is met zenden/ontvangen. */
	void zetKlaar()
	{
		count=Grootte;
	};
	
private:
	UInt32 count;

};

class STM32Controller;  /* voorwaarts */

class STM32Uart
{
public: 
    STM32Uart();

    static int putCh(const char c,
                     FILE *file=NULL);

    static int getCh (FILE *file=NULL);


protected:

		static const UInt32 USART1_DR_Base=0x40013804;
	  static const UInt32 USART2_DR_Base=0x40004404;



};


class STM32DAC
{
public:

	typedef enum
	{
		DAC1Keuze=0,
		DAC2Keuze,
		DACAAantalKeuzen
	} DACKeuze;
	
    STM32DAC(const DACKeuze);

    void set(const UInt16 value) const;

protected:

private:
  const DACKeuze dacKeuze;
};


class STM32ADC
{
public:

	typedef enum
	{
		ADC1Kanaal1=0,
		ADC1Kanaal2,
		ADC1Kanaal3,
		ADC1Kanaal6, 
		ADC1AantalKanalen
	} ADCKeuze;
	
		/*! @brief Constructor met samplllling tijd in milliseconden */
    STM32ADC(const ADCKeuze);	 

    void startStopBemonstering(const bool);

    void zetSampTijd(const UInt16 st);   /* sampling tijd in millisecs */
    void zetSampTijd(const UInt32 st);     /* sampling tijd in microsecs */


    UInt16 readAnalog() const;

protected:

private:
    TIM_TypeDef * const timer;
  	const UInt32 timBus;
		const UInt32 adcTimTrigger;

		const ADCKeuze adcKeuze;
};



#endif // Perifs
