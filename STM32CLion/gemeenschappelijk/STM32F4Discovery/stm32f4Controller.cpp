/******************************************************************************
 * Project        : HAN ESE Project 3 - Regeltechniek
 * File           : Basisklasse en startup routines van de STM32

 * Copyright      : 2010-2011 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 2.0 - September 2011
    > begin .

******************************************************************************/
/* $Id: stm32f4Controller.cpp 2279 2016-08-29 15:00:23Z ewout $     */


#include <algdef.h>
#include "stm32f4Controller.h"
#include "stm32f4_discovery.h"

#include <assert.h>

/******************************************************************************
  Global variables
******************************************************************************/


#ifdef __cplusplus
extern "C" {
#endif       
/******************************************************************************
  Cortex-M3 Processor Exceptions Handlers
******************************************************************************/

// ----------------------------------------------------------------------------
// This function handles NMI exception.
// ----------------------------------------------------------------------------
    void NMI_Handler(void)
    {
        while(1){;}
    }

// ----------------------------------------------------------------------------
// This function handles Hard Fault exception.
// ----------------------------------------------------------------------------
    void HardFault_Handler(void)
    {
        // Go to infinite loop when Hard Fault exception occurs
        while(1){;}
    }

// ----------------------------------------------------------------------------
// This function handles Memory Manage exception.
// ----------------------------------------------------------------------------
    void MemManage_Handler(void)
    {
        // Go to infinite loop when Memory Manage exception occurs
        while(1){;}
    }

// ----------------------------------------------------------------------------
// This function handles Bus Fault exception.
// ----------------------------------------------------------------------------
    void BusFault_Handler(void)
    {
        // Go to infinite loop when Bus Fault exception occurs
        while(1){;}
    }

// ----------------------------------------------------------------------------
// This function handles Usage Fault exception.
// ----------------------------------------------------------------------------
    void UsageFault_Handler(void)
    {
        // Go to infinite loop when Usage Fault exception occurs
        while(1){;}
    }

// ----------------------------------------------------------------------------
// This function handles SVCall exception.
// ----------------------------------------------------------------------------
    void SVC_Handler(void)
    {
    }

// ----------------------------------------------------------------------------
// This function handles Debug Monitor exception.
// ----------------------------------------------------------------------------
    void DebugMon_Handler(void)
    {
    }

// ----------------------------------------------------------------------------
// This function handles PendSV_Handler exception.
// ----------------------------------------------------------------------------
    void PendSV_Handler(void)
    {
    }

// ----------------------------------------------------------------------------
// This function handles SysTick Handler.
// ----------------------------------------------------------------------------
void SysTick_Handler(void)
{
     static bool stand=false;

        if (stand == true)
            STM_EVAL_LEDOn(LED3) ;
        else
            STM_EVAL_LEDOff(LED3);
  
    }


 void EXTI0_IRQHandler(void)
 {
	STM32F4Controller::knopGedrukt();
	EXTI_ClearITPendingBit(EXTI_Line0);
 }



#ifdef __cplusplus
}
#endif


bool STM32F4Controller::knop=false;

STM32F4Controller::STM32F4Controller()
{
    /*!< At this stage the microcontroller clock setting is already configured, 
      this is done through SystemInit() function which is called from startup
      file (startup_stm32f4xx.s) before to branch to application main.
      To reconfigure the default setting of SystemInit() function, refer to
      system_stm32f4xx.c file
    */
    __disable_irq();

    // --------------------------------------------------------------------------

    // Initialize Leds LD3 and LD4 mounted on STM32VLDISCOVERY board  
    for (Teller i=0;i<4;i++)
    {
        STM_EVAL_LEDInit((Led_TypeDef)i);
        STM_EVAL_LEDOff((Led_TypeDef)i);
    }
    // --------------------------------------------------------------------------
    // Initialize Push Button
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

    __enable_irq();


}


/*******************************************************************************
 * Function Name  : RCC_Configuration
 * Description    : Configures the different system clocks.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void STM32F4Controller::RCCInit(void)
{

    /* Alle benodigde klokken in dit project */ 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1|RCC_AHB1Periph_GPIOA, ENABLE); 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC , ENABLE); 

    RCC_APB2PeriphClockCmd((RCC_APB2Periph_USART1 |
                            RCC_APB2Periph_ADC1), ENABLE );

}


char STM32F4Controller::wachtOpDesktopCommando() const
{
    const char desktop = uart.wachtOpDesktopCommando();

    return(desktop);
}

void STM32F4Controller::LED(const STM32F4Controller::Kleur kleur,
                            const Schakelaar stand) const
{
    const Led_TypeDef kl = static_cast<Led_TypeDef>(kleur);
    switch(stand)
    {

        case SchakelaarUit:
            STM_EVAL_LEDOff(kl);
            break;
        case SchakelaarAan:
            STM_EVAL_LEDOn(kl);
            break;
        case SchakelaarOm:
            STM_EVAL_LEDToggle(kl);
            break;
        default:
            assert(0);
    }
}



void STM32F4Controller::debounce() const
{
	   /* ruwe debounce controle */
    volatile unsigned int debounceWaarde =0;
    for (volatile unsigned int i=0;i<999;i++)
        debounceWaarde++;
}

void STM32F4Controller::wachtOpKnop()
{

    resetKnop();

    /* wacht op interrupt van knop drukken */
    do
    {
        debounce();

    } while (false == geefKnopStand());

}


void STM32F4Controller::resetKnop()
{
    knop=false;
}

/* wordt aangeroepen in de EXTI0 IRQ handler */
void STM32F4Controller::knopGedrukt()
{
    knop = true;
}

bool STM32F4Controller::geefKnopStand() const
{
    return(knop);
}


