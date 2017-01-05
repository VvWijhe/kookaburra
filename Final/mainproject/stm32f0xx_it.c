

/* Includes ------------------------------------------------------------------*/
#include <airplane.h>
#include <counter.h>
#include <stm32f0_discovery.h>
#include "stm32f0xx_it.h"

#define RCHIGH 5.0
#define RCLOW 4.0
#define MAXATTEMPTS 10



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
int count = 0;

void TIM3_IRQHandler() {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
//        previousAltitude = currentAltitude;
//        currentAltitude = Airplane::getAltitude();
//        verticalSpeed = (float) ((currentAltitude - previousAltitude) / 0.2);

        // Delay = (count - 1) / TIM3 freq
        if (count++ == 4) {
            if(verticalSpeed != 0){
                TIM_Cmd(TIM17, DISABLE);
            } else {
                Timer::setTim17((int)1000*10);
            }
            count = 0;
        }
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
    }
}

/**
  * @brief  This function handles TIM15 interrupt for the PWM capture compare and the switch for the autopilot.
  * @param  None
  * @retval None
  */
void TIM15_IRQHandler(void) {
    __IO uint32_t IC2Value = 0;
    __IO uint32_t DutyCycle = 0;

    flightMode = MANUAL_M; //the flightMode the plane should work with, auto or manual.

    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM15, TIM_IT_CC1);

    /* Get the Input Capture value */
    IC2Value = TIM_GetCapture2(TIM15);

    if (IC2Value > TIM_GetCapture1(TIM15)) {
        /* Duty cycle computation */
        DutyCycle = IC2Value - TIM_GetCapture1(TIM15);
        PrevDutyCycle = DutyCycle;
        currentDutyCycle = (float) ((DutyCycle - 49200) / 163.2);
        if (currentDutyCycle < 0) {
            currentDutyCycle += 301.9;
        }
    } else {
        DutyCycle = (uint32_t) PrevDutyCycle;
    }

    //from here on the flightplanner switch is processed.
    // NOTE: MAXATTEMPTS, RCHIGH and RCLOW are still trivial, give them actual values.
    if (StepCount == 0 && currentDutyCycle > RCHIGH && flightMode == MANUAL_M) //first up
    {
        StepCount++;
        Attempts = 0;
    } else if (StepCount == 1 && currentDutyCycle < RCLOW && Attempts < MAXATTEMPTS && flightMode == MANUAL_M) //then down
    {
        StepCount++;
        Attempts = 0;
    } else if (StepCount == 2 && currentDutyCycle > RCHIGH && Attempts < MAXATTEMPTS &&
               flightMode == MANUAL_M) //up again, enable Flightplanner
    {
        flightMode = AUTOPILOT_M;
        Attempts = 0;
    } else if (flightMode == AUTOPILOT_M && Attempts > MAXATTEMPTS &&
               currentDutyCycle < RCLOW) //down when Flightplanner is active disables it
    {
        flightMode = MANUAL_M;
        Attempts = 0;
        StepCount = 0;
    }

    Attempts++;
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
    }
}

/**
  * @}
  */

/**
  * @}
  */
