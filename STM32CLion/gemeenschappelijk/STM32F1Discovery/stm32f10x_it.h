/******************************************************************************
 * Project        : HAN ESE Project 3 - Regeltechniek
 * File           : This file contains the function prototypes of the interrupt
 *                  handlers
 * Copyright      : 2011 HAN Embedded Systems Engineering
 ******************************************************************************
  Change History:

    Version 1.0 - april 2011
    > Initial revision

******************************************************************************/
#ifndef _STM32F10x_IT_H
#define _STM32F10x_IT_H

#include "stm32f10x.h"

/******************************************************************************
  Defines
******************************************************************************/

/******************************************************************************
  Function prototypes
******************************************************************************/
void NMI_Handler(void);

void HardFault_Handler(void);

void MemManage_Handler(void);

void BusFault_Handler(void);

void UsageFault_Handler(void);

void SVC_Handler(void);

void DebugMon_Handler(void);

void PendSV_Handler(void);

void SysTick_Handler(void);

void USART1_IRQHandler(void);

#endif /* _STM32F10x_IT_H */

/******************************************************************************
  EOF
******************************************************************************/
