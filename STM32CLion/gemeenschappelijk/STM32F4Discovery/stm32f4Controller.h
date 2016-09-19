/******************************************************************************
 * Project        : HAN ESE 
 * File           : Basisklasse en startup routines van de STM32F4

 * Copyright      : 2010-2013 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 2.0 - September 2011
    > STM32 versie van Fuzzy en Regeltechniek opdracht.

******************************************************************************/

#ifndef stm32F4Controller_h
#define stm32F4Controller_h

#include "stm32f4Perifs.h"

/******************************************************************************
  Defines
******************************************************************************/
#define CR        ('\r')
#define LF        ('\n')
#define BACKSPACE ('\b')
#define SPACE     (' ')
#define ESC       (0x1B)


#include <string.h>
#include <assert.h>

#include "algdef.h"

/******************************************************************************
  Defines
******************************************************************************/

// Uncomment the line below to expanse the "assert_param" macro in the 
// Standard Peripheral Library drivers code
// #define USE_FULL_ASSERT  (1)

#ifdef USE_FULL_ASSERT

// The assert_param macro is used for function's parameters check.
//   expr: If expr is false, it calls assert_failed function
//   which reports the name of the source file and the source
//   line number of the call that failed. 
//   If expr is true, it returns no value.

inline void assert_param(const bool expr) 
{
    if (expr==false) 
        assert_failed((uint8_t *)__FILE__, __LINE__);
};

void assert_failed(uint8_t* file, uint32_t line);

#else

  #define assert_param(expr) ((void)0)

#endif // USE_FULL_ASSERT


/******************************************************************************
  Global variables
******************************************************************************/
extern uint32_t trigger;

extern uint32_t rx_ready;
extern uint8_t  rx_buffer[10];

/******************************************************************************
  Defines
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  Function prototypes
******************************************************************************/
void NMI_Handler        (void);
void HardFault_Handler  (void);
void MemManage_Handler  (void);
void BusFault_Handler   (void);
void UsageFault_Handler (void);
void SVC_Handler        (void);
void DebugMon_Handler   (void);
void PendSV_Handler     (void);
void SysTick_Handler    (void);
void USART1_IRQHandler  (void);

#ifdef __cplusplus
}
#endif

class STM32F4Controller
{
public:
    STM32F4Controller();


    typedef enum
    {
        Groen=0,
        Rood,
        Oranje,
        Blauw
    } Kleur;

    void LED(const Kleur,
             const Schakelaar) const;
	
    /* wordt aangeroepen in de EXTI0 IRQ handler */
    static void knopGedrukt();
	
    void wachtOpKnop();
	
protected:

    STM32Uart uart;
    STM32DAC dac;
    STM32ADC adc;

    char wachtOpDesktopCommando() const;

    bool geefKnopStand() const;
    void resetKnop();


private:
    void debounce() const;

    void RCCInit();

    static bool knop;	  /* bevat de stand van de drukknop als ware het een knop met geheugen */
};

#endif /* stm32Controller_h */
