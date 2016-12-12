

/* Includes ------------------------------------------------------------------*/
#include <airplane.h>
#include <flash.h>
#include "stm32f0xx_it.h"
#include "usart.h"

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void) {
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void) {
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void) {
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void) {
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void) {

}

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler() {
    if((rxBuffer[indexBuffer] = (uint8_t) (USART_ReceiveData(USART1))) != '\r'){
        indexBuffer++;
    } else {
        indexBuffer = 0;

        /// TBD: convert string to number
//        Flash memory;
//        memory.write32(EEPROM_START_ADDRESS, 1);
    }
}

/**
  * @}
  */

/**
  * @}
  */
