/******************************************************************************
 * Project        : HAN ESE Project 3 - Regeltechniek
 * File           : Basisklasse en startup routines van de STM32

 * Copyright      : 2010-2011 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 2.0 - September 2011
    > STM32 versie van Fuzzy en Regeltechniek opdracht.

******************************************************************************/

#ifndef stm32Controller_h
#define stm32Controller_h

#include <stm32f10x.h>
#include <stm32f10x_wwdg.h>

#include "stm32Periphs.h"


/******************************************************************************
  Global variables
******************************************************************************/
extern uint32_t trigger;

extern uint32_t rx_ready;
extern uint8_t  rx_buffer[10];

#define ALTIJD   while(1)

/* indeling :
   TIM2 = ADC timer 
   TIM3 = knipper LED 
   TIM4 = wacht timer */

class STM32Controller
{
public:
    STM32Controller();
		
    typedef enum
    {
        KnipperLED_stop=0,
        KnipperLED_langzaam,
        KnipperLED_middel,
        KnipperLED_snel,
        KnipperLED_zeerSnel,
        KnipperLED_Laatste
    } KnipperLEDStand;
		
    /*! @brief Schakel LED 3 naar een knipperstand.
      @param de knipperstand */
    void knipperLEDStand(const KnipperLEDStand) const;
				
    /*! @brief schakel LED4 */
    void statusLED(const bool stand) const;	
    /*! @brief Alias van bovenstaande funktie met ander argument. */
    void statusLED(const Schakelaar ) const;
	
    /*! @brief wacht in stop mode/ slaap mode op knopdruk.
     * @param stopMode = true --> wacht in STOP mode, false --> wacht in SLEEP mode */
    void wachtOpKnop(const bool stopMode=false);

    /*! @brief wacht in stop mode/ slaap mode
     * @param stopMode = true --> wacht in STOP mode, false --> wacht in SLEEP mode */
    void wachtFunktie(const bool stopMode);
				
    /*! @brief wacht funktie gebaseerd op timer 4
     *  @param  Wachttijd in millisecs.
     *  @param Wacht in slaapmode (true = sleepmode, false = polling) */
    void wachtFunktie(const unsigned short,           
                      const bool slaap=true) const;  
		
    /*! @brief Stel de Independent wachtdog in */
    void waakhond(const UInt16 prescaler=8,
                  const UInt16 venster=80) const;
													
    /*! @brief print een tekst via het debug kanaal */
    static UInt32 ITMPrint(const UInt32 ch);
    static void ITMPrint(const char *);   
    /*! @brief  Deze funkties worden aangeroepen in de EXTI0 IRQ/TIM2IRQ handler */
    static void knopGedrukt();
    static void timer2IsAfgelopen();
			
protected:

    bool geefKnopStand() const;
    void resetKnop();

    void enableSWO(const UInt32 targetDiv,const UInt32 itmPort=0);
   
private:
    void timer4WachtInit();
    void knipperLEDInit();
    void slaapMode() const;

    void SYSCLKConfig_STOP(void);  /* herstel HSE/PLL na stop mode */

    ErrorStatus HSEStartUpStatus;

    static const UInt16 knipperStanden[];
    static bool knop;	  /* bevat de stand van de drukknop als ware het een knop met geheugen */
    static bool timer2Afgelopen;
};

#endif /* stm32Controller_h */
