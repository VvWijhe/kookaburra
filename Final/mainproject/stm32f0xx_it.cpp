/* Includes ------------------------------------------------------------------*/
#include "airplane.h"
#include "stm32f0xx_it.h"

#define RCHIGH 86
#define RCLOW 61
#define MAXATTEMPTS 100

typedef enum {
    OFF,
    ON
} FlightPlanner;

FlightPlanner Newstate = OFF;

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
int whichAltitutde = 0;

void USART1_IRQHandler() {
    if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
        rxBuffer[indexBuffer] = (char) USART_ReceiveData(USART1);
        if (rxBuffer[indexBuffer] != '\r') {
            indexBuffer++;
        } else {
            rxBuffer[indexBuffer] = '\0';
            uart << "String is: " << rxBuffer << "\n";

            if(whichAltitutde == 0){
                altitude1 = atoi(rxBuffer);
                whichAltitutde = 1;
            } else if(whichAltitutde == 1){
                altitude2 = atoi(rxBuffer);
                whichAltitutde = 0;
            }


            // clear buffer
            memset(&rxBuffer[0], 0, sizeof(rxBuffer));
            indexBuffer = 0;
            /// TBD: convert string to number
        }

        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
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
        previousAltitude = currentAltitude;
        currentAltitude = barometer.getAltitude();
        verticalSpeed = (float) ((currentAltitude - previousAltitude) / 0.2);

        // Delay = (count - 1) / TIM3 freq
        if (count++ == 4) {
            if (verticalSpeed < 0.2 && verticalSpeed > -0.2) {
                RGB::disable();
            } else {
                if (verticalSpeed < 0) {
                    verticalSpeed *= -1;
                }
                RGB::setFrequency(verticalSpeed);
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

        Time::seconds++;
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
        const double gConversion = 3.9;
        accelGyroDataRaw_t accelGyroDataRaw;

        accelerometer.getRawAccelGyro(&accelGyroDataRaw);

        double accelerationX = (accelGyroDataRaw.Ax * gConversion);
        double accelerationY = (accelGyroDataRaw.Ay * gConversion);
        double accelerationZ = (accelGyroDataRaw.Az * gConversion);

        currentPitch = (int) (180 * atan (accelerationX / sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) / M_PI);
    }
}

/**
  * @brief  This function handles TIM15 interrupt for the PWM capture compare and the switch for the autopilot.
  * @param  None
  * @retval None
  */
int nChannel = 0;

void TIM15_IRQHandler() {
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM15, TIM_IT_CC1);

    /* Get the Input Capture value */
    IC2Value = TIM_GetCapture2(TIM15);
    IC1Value = TIM_GetCapture1(TIM15);
    if ( /*IC1Value - IC2Value < 10000*/ 1 )
    {
        temp = IC1Value - IC2Value;
        /* Duty Cycle computation */
        DutyCycle = IC1Value - IC2Value;
        if ( DutyCycle > 50000 )
        {
            DutyCyclePC = (float)( DutyCycle - 49200 ) / 163.2;
        }
        else
        {
            DutyCyclePC = (float)DutyCycle / 163.2;
        }
        if ( DutyCyclePC < 0 || DutyCyclePC > 99 )
        {
            DutyCyclePC = PrevDutyCycle;
        }
        PrevDutyCycle = DutyCyclePC;
    }
    else
    {
        DutyCyclePC = PrevDutyCycle;
    }

    //from here on the flightplanner switch is processed.
    // NOTE: MAXATTEMPTS, RCHIGH and RCLOW are still trivial, give them actual values



    if ( StepCount == 0 && DutyCyclePC < RCLOW && Newstate == OFF ) //first up
    {
        StepCount++;
        Attempts = 0;
    }
    else if ( StepCount == 1 && DutyCyclePC > RCHIGH && Attempts < MAXATTEMPTS && Newstate == OFF ) //then down
    {
        StepCount++;
        Attempts = 0;
    }
    else if ( StepCount == 2 && DutyCyclePC < RCLOW && Attempts < MAXATTEMPTS && Newstate == OFF ) //up again, enable Flightplanner
    {
        Newstate = ON;
        Attempts = 0;
        StepCount = 0;
        flightMode = AUTOPILOT_M;
        GPIO_SetBits(GPIOC, AUTOPILOT_ENABLE_PIN);
    }
    else if ( Newstate == ON && Attempts > MAXATTEMPTS && DutyCyclePC > RCHIGH ) //down when Flightplanner is active, disable it
    {
        flightMode = MANUAL_M;
        Newstate = OFF;
        Attempts = 0;
        GPIO_ResetBits(GPIOC, AUTOPILOT_ENABLE_PIN);
    }

    Attempts++;
}




/**
  * @}
  */

/**
  * @}
  */
