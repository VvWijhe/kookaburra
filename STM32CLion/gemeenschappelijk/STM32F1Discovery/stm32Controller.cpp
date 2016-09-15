/******************************************************************************
 * Project        : HAN ESE Project 3 - Regeltechniek
 * File           : Basisklasse en startup routines van de STM32

 * Copyright      : 2010-2013 HAN Embedded Systems Engineering Ewout Boks en Hugo Arends
 ******************************************************************************
  Change History:

    Version 2.0 - September 2011
    > begin .
 $Id: stm32Controller.cpp 2241 2016-06-09 19:21:18Z ewout $     
******************************************************************************/

#include <stm32f10x_pwr.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_dbgmcu.h> 
#include <stm32f10x_iwdg.h>

#include <assert.h>

STM32Controller::STM32Controller() {
    __disable_irq();

    enableSWO(8);  /* 24/8 = 3 MHz output */

    // --------------------------------------------------------------------------
    // Initialize Leds LD3 and LD4 mounted on STM32VLDISCOVERY board  
    STM32vldiscovery_LEDInit(LED3);
    STM32vldiscovery_LEDOff(LED3);
    STM32vldiscovery_LEDInit(LED4);
    STM32vldiscovery_LEDOff(LED4);

    // --------------------------------------------------------------------------
    // Initialize Push Button
    STM32vldiscovery_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);

    /* tijdens slaap en stop moet clk voor debug blijven draaien */
#ifndef NDEBUG
    DBGMCU_Config(DBGMCU_SLEEP, ENABLE);
    DBGMCU_Config(DBGMCU_STOP, ENABLE);
    DBGMCU_Config(DBGMCU_TIM2_STOP, ENABLE);
#endif

    timer4WachtInit();
    knipperLEDInit();

    __enable_irq();
}

void STM32Controller::timer4WachtInit() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    /* TIM4 moederclock enable = APB1/2 = 12 MHz */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /* Timer op 1 kHz --> Presc*Period/(2*12E6) = 1/1000 
     * presc = 24E3, Timer periode = msecs
     */

    TIM_DeInit(TIM4);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 65535;    /* voorlopig */
    TIM_TimeBaseStructure.TIM_Prescaler = 24000;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure);

    /* TIM 4 interrupt */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
}

void STM32Controller::knipperLEDInit() {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* LED3 --> PC9 --> TIM3_CH4 */
    /* TIM3 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOA Configuration: TIM3 CH4 (PC9)  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);

    /* Timer op 2 Hz --> Presc*Period/24E6 = 1/4 
     * presc = 1E3, PWM periode = 12E3, 50% duty cycle = 6E3 
     */

    TIM_DeInit(TIM3);
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 12000;
    TIM_TimeBaseStructure.TIM_Prescaler = 1000;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 6000;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM3, ENABLE);

    TIM_Cmd(TIM3, ENABLE);

}

const UInt16 STM32Controller::knipperStanden[STM32Controller::KnipperLED_Laatste] =
        {
                0,
                12000,
                6000,
                3000,
                1000
        };

void STM32Controller::knipperLEDStand(const STM32Controller::KnipperLEDStand stand) const {
    assert(stand < KnipperLED_Laatste);

    if (KnipperLED_stop == stand) {
        TIM_Cmd(TIM3, DISABLE);
    } else {
        const UInt16 nieuweStand = knipperStanden[stand];
        TIM_Cmd(TIM3, ENABLE);
        TIM_SetAutoreload(TIM3, 2 * nieuweStand);
        TIM_SetCompare4(TIM3, nieuweStand);
    }
}

void STM32Controller::statusLED(const bool stand) const {
    const Schakelaar knop = ((true == stand) ? SchakelaarAan : SchakelaarUit);

    statusLED(knop);
}

void STM32Controller::statusLED(const Schakelaar stand) const {
    switch (stand) {
        case SchakelaarUit:
            STM32vldiscovery_LEDOff(LED4);
            break;
        case SchakelaarAan:
            STM32vldiscovery_LEDOn(LED4);
            break;
        case SchakelaarOm:
            STM32vldiscovery_LEDToggle(LED4);
            break;
        default:
            assert(0);
    }
}

void STM32Controller::wachtOpKnop(const bool stopMode) {

    resetKnop();

    wachtFunktie(stopMode);

}

void STM32Controller::wachtFunktie(const bool stopMode) {
    if (true == stopMode) {
        PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);

        /* STM32 gaat verder na knop interrupt */
        SYSCLKConfig_STOP();  /* breng PLL en HSE weer tot leven */
    } else  /* wacht op interrupt */
    {
        slaapMode();
    }
}

void STM32Controller::wachtFunktie(const unsigned short msecs,
                                   const bool slaap) const {
    TIM_SetCounter(TIM4, 0x0);
    TIM_SetCompare1(TIM4, msecs);
    timer2Afgelopen = false;
    TIM_Cmd(TIM4, ENABLE);

    do {
        if (true == slaap)
            slaapMode();
    } while (false == timer2Afgelopen);
}

bool STM32Controller::timer2Afgelopen = false;

void STM32Controller::timer2IsAfgelopen() {
    timer2Afgelopen = true;
}

bool STM32Controller::knop = false;

void STM32Controller::resetKnop() {
    knop = false;
}

void STM32Controller::slaapMode() const {
    /* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM   )
    __force_stores();
#endif
    /* Request Wait For Interrupt */
    __WFI();
}

/* ##################################    Debug Output  function  ############################################ */

#define ITM_ENA (*(volatile unsigned int*)0xE0000E00) // ITM Enable
#define ITM_TPR (*(volatile unsigned int*)0xE0000E40) // Trace Privilege Register
#define ITM_TCR (*(volatile unsigned int*)0xE0000E80) // ITM Trace Control Reg.
#define ITM_LSR (*(volatile unsigned int*)0xE0000FB0) // ITM Lock Status Register
#define DHCSR (*(volatile unsigned int*)0xE000EDF0) // Debug register
#define DEMCR (*(volatile unsigned int*)0xE000EDFC) // Debug register
#define TPIU_ACPR (*(volatile unsigned int*)0xE0040010) // Async Clock + prescaler register
#define TPIU_SPPR (*(volatile unsigned int*)0xE00400F0) // Selected Pin Protocol Register
#define DWT_CTRL (*(volatile unsigned int*)0xE0001000) // DWT Control Register
#define FFCR (*(volatile unsigned int*)0xE0040304) // Formatter and flush

// Control Register
//const UInt32 CortexM3Processor::_ITMPort = 0; // The stimulus port from which SWO data is received and displayed.
//const UInt32 CortexM3Processor::TargetDiv = 1;// Has to be calculated according to the CPU speed and the output baud rate
void STM32Controller::enableSWO(const UInt32 targetDiv, const UInt32 itmPort) {

/* code is afkomstig van Segger J-link handleiding pdf,
 * bladzijde 74.
 */

    UInt32 StimulusRegs = ITM_ENA & (~(1 << itmPort));
//
// Enable access to SWO registers
//
    DEMCR |= (1 << 24);
    ITM_LSR = 0xC5ACCE55;
//
// Initially disable ITM and stimulus port
// To make sure that nothing is transferred via SWO
// when changing the SWO prescaler etc.
//
    ITM_ENA = StimulusRegs; // Disable ITM stimulus port
    ITM_TCR = 0; // Disable ITM
//
// Initialize SWO (prescaler, etc.)
//
    TPIU_SPPR = 0x00000002; // Select NRZ mode
    TPIU_ACPR = targetDiv - 1; // Example: 72/48 = 1,5 MHz
    ITM_TPR = 0x00000000;
    DWT_CTRL = 0x400003FE;
    FFCR = 0x00000100;
//
// Enable ITM and stimulus port
//
    ITM_TCR = 0x1000D; // Enable ITM
    ITM_ENA = StimulusRegs | (1 << itmPort); // Enable ITM stimulus port

}


UInt32 STM32Controller::ITMPrint(const UInt32 ch) {
    return (::ITM_SendChar(ch));
}

void STM32Controller::ITMPrint(char const *const buffer) {
    const char *bufPtr = buffer;
    char teken;

    do {
        ITMPrint(teken = *(bufPtr++));
    } while (teken != '\0');
}


/* wordt aangeroepen in de EXTI0 IRQ handler */
void STM32Controller::knopGedrukt() {
    knop = true;
}

bool STM32Controller::geefKnopStand() const {
    return (knop);
}


/**
 * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL
 *         and select PLL as system clock source.
 * @param  None
 * @retval None
 */
void STM32Controller::SYSCLKConfig_STOP(void) {
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS) {

#ifdef STM32F10X_CL
        /* Enable PLL2 */
        RCC_PLL2Cmd(ENABLE);

        /* Wait till PLL2 is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
        {
        }

#endif

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08) {
        }
    }
}

void STM32Controller::waakhond(const UInt16 prescaler,
                               const UInt16 venster) const {
    /* Enable WWDG clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

    IWDG_SetPrescaler(prescaler);
    IWDG_SetReload(venster);

    IWDG_Enable();
}

/******************************************************************************
  Global variables
******************************************************************************/

/* C++ name mangling mag niet de interrupt handler namen veranderen, 
 * deze staan in een assembly bestand zo genoemd.
 * Derhalve hieronder met C-Linking compileren.
 */
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
  Cortex-M3 Processor Exceptions Handlers
******************************************************************************/

// ----------------------------------------------------------------------------
// This function handles NMI exception.
// ----------------------------------------------------------------------------
void NMI_Handler(void) {
    while (1) { ; }
}

// ----------------------------------------------------------------------------
// This function handles Hard Fault exception.
// ----------------------------------------------------------------------------
void HardFault_Handler(void) {
    // Go to infinite loop when Hard Fault exception occurs
    while (1) { ; }
}

// ----------------------------------------------------------------------------
// This function handles Memory Manage exception.
// ----------------------------------------------------------------------------
void MemManage_Handler(void) {
    // Go to infinite loop when Memory Manage exception occurs
    while (1) { ; }
}

// ----------------------------------------------------------------------------
// This function handles Bus Fault exception.
// ----------------------------------------------------------------------------
void BusFault_Handler(void) {
    // Go to infinite loop when Bus Fault exception occurs
    while (1) { ; }
}

// ----------------------------------------------------------------------------
// This function handles Usage Fault exception.
// ----------------------------------------------------------------------------
void UsageFault_Handler(void) {
    // Go to infinite loop when Usage Fault exception occurs
    while (1) { ; }
}

// ----------------------------------------------------------------------------
// This function handles SVCall exception.
// ----------------------------------------------------------------------------
void SVC_Handler(void) {
}

// ----------------------------------------------------------------------------
// This function handles Debug Monitor exception.
// ----------------------------------------------------------------------------
void DebugMon_Handler(void) {
}

// ----------------------------------------------------------------------------
// This function handles PendSV_Handler exception.
// ----------------------------------------------------------------------------
void PendSV_Handler(void) {
}

// ----------------------------------------------------------------------------
// This function handles SysTick Handler.
// ----------------------------------------------------------------------------
void SysTick_Handler(void) {
    static bool stand = false;

    if (stand == true)
        STM32vldiscovery_LEDOn(LED3);
    else
        STM32vldiscovery_LEDOff(LED3);
}

/* Knop druk interrupt */
void EXTI0_IRQHandler(void) {
    STM32Controller::knopGedrukt();
    EXTI_ClearITPendingBit(EXTI_Line0);
}



/*		void TIM3_IRQHandler(void)
		{
                if (RESET != TIM_GetITStatus(TIM3,TIM_IT_Update))
                {
                TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
                STM32vldiscovery_LEDToggle(LED3);
                }
		}
*/
/* wacht timer */
void TIM4_IRQHandler(void) {
    if (RESET != TIM_GetITStatus(TIM4, TIM_IT_CC1)) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
        STM32Controller::timer2IsAfgelopen();
        TIM_Cmd(TIM4, DISABLE);
    }
}

void assert_failed(uint8_t *file, uint32_t line) {

    while (1);
}

#ifdef __cplusplus
}
#endif
