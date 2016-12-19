

/* Includes ------------------------------------------------------------------*/
#include <airplane.h>
#include <stm32f0_discovery.h>
#include <counter.h>
#include "stm32f0xx_it.h"

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
  * @brief  This function handles USART interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler() {
    if ((rxBuffer[indexBuffer] = (uint8_t) (USART_ReceiveData(USART1))) != '\r') {
        indexBuffer++;
    } else {
        indexBuffer = 0;

        /// TBD: convert string to number
    }
}

/**
  * @brief  This function handles TIM3 interrupt request for altitude update.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler() {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        STM_EVAL_LEDToggle(LED4);

//        previousAltitude = currentAltitude;
//        currentAltitude = Airplane::getAltitude();
//        verticalSpeed = (float) ((currentAltitude - previousAltitude) / 0.2);
        verticalSpeed = 2;
        if (verticalSpeed < 0) {
            verticalSpeed = verticalSpeed * -1;
        }
        int Anita = static_cast<int>(verticalSpeed);
        Timer::setTim17(verticalSpeed);

    }
}

/**
  * @brief  This function handles TIM14 interrupt request for the stopwatch.
  * @param  None
  * @retval None
  */
void TIM14_IRQHandler() {
    if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM14, TIM_IT_Update);

        Timer::incrementTime(Time::seconds);
        STM_EVAL_LEDToggle(LED4);
    }
}

/**
  * @brief  This function handles TIM16 interrupt request for the pitch update.
  * @param  None
  * @retval None
  */
void TIM16_IRQHandler(void) {
    if (TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);

        //currentAltitude = Airplane::getPitch();
        //STM_EVAL_LEDToggle(LED3);
    }
}

void TIM17_IRQHandler(void) {
    if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM17, TIM_IT_Update);

        switch (ledColor) {

            case LEDGREEN:
                Airplane::setColor(LEDGREEN);
                break;

            case LEDYELLOW:
                Airplane::setColor(LEDYELLOW);
                break;

            case LEDRED:
                Airplane::setColor(LEDRED);
                break;

            default:
                break;

        }

        //STM_EVAL_LEDToggle(LED3);
    }
}

/**
  * @}
  */

/**
  * @}
  */
