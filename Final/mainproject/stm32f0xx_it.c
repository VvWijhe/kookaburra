/* Includes ------------------------------------------------------------------*/
#include "airplane.h"
#include <stm32f0_discovery.h>
#include <pwm.h>
#include "rgbdriver.h"
#include "stm32f0xx_it.h"

#define RCHIGH 9.2
#define RCLOW 5.0
#define MAXATTEMPTS 20

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#ifdef STM32F072
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
 #define FLASH_USER_START_ADDR   ((uint32_t)0x08009000)   /* Start @ of user Flash area */
 #define FLASH_USER_END_ADDR     ((uint32_t)0x08020000)   /* End @ of user Flash area */
#elif defined (STM32F091)
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000800)   /* FLASH Page Size */
 #define FLASH_USER_START_ADDR   ((uint32_t)0x08009000)   /* Start @ of user Flash area */
 #define FLASH_USER_END_ADDR     ((uint32_t)0x08040000)   /* End @ of user Flash area */
#else
#define FLASH_PAGE_SIZE         ((uint32_t)0x00000400)   /* FLASH Page Size */
#define FLASH_USER_START_ADDR   ((uint32_t)0x08006000)   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ((uint32_t)0x08007000)   /* End @ of user Flash area */
#endif /* STM32F072 */
uint8_t WriteToFlash( uint16_t HeightOne, uint16_t HeightTwo );
uint16_t ReadFromFlash( uint8_t H1orH2 );
uint32_t EraseCounter = 0x00, Address = 0x00;
uint32_t Data = 0x3210ABCD;
uint32_t NbrOfPage = 0x00;
__IO FLASH_Status FLASHStatus = FLASH_COMPLETE;
__IO TestStatus MemoryProgramStatus = PASSED;

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
            if (verticalSpeed < 0.2 && verticalSpeed > -0.2) {
                //RGB::disable();
            } else if(verticalSpeed < 0) {
                verticalSpeed *= -1;
            } else {
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
int nChannel = 0;

void TIM15_IRQHandler() {
    /* Clear TIM2 Capture compare interrupt pending bit */
    TIM_ClearITPendingBit(TIM15, TIM_IT_CC1);

    __IO uint32_t IC2Value = 0;
    __IO uint32_t DutyCycle = 0;

    float f = SystemCoreClock / TIM_GetCapture2(TIM15);
    float T = 1 / f;

    flightMode = MANUAL_M; //the flightMode the plane should work with, auto or manual.

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

    // TSync > 12 ms
    // Set channel data if period is smaller than 10 ms
    if(T < 0.01) {
        if (nChannel == 0) {
            ppmData.channel1 = (int) (T * 1000000);
            nChannel++;
        } else if(nChannel == 1){
            ppmData.channel2 = (int) (T * 1000000);
            nChannel++;
        } else if(nChannel == 2){
            ppmData.channel3 = (int) (T * 1000000);
            nChannel++;
        } else if(nChannel == 3){
            ppmData.channel4 = (int) (T * 1000000);
            nChannel++;
        }
    } else {
        nChannel = 0;
    }

    //from here on the flightplanner switch is processed.
    // NOTE: MAXATTEMPTS, RCHIGH and RCLOW are still trivial, give them actual values.
    if (StepCount == 0 && currentDutyCycle > RCHIGH && flightMode == MANUAL_M) //first up
    {
        StepCount++;
        Attempts = 0;
    } else if (StepCount == 1 && currentDutyCycle < RCLOW && Attempts < MAXATTEMPTS &&
               flightMode == MANUAL_M) //then down
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

//function that writes the heights to flash
uint8_t WriteToFlash( uint16_t HeightOne, uint16_t HeightTwo )
{
    uint8_t SuccessCode = 0;
    uint32_t DATA_32;

    DATA_32 = HeightOne;
    DATA_32 = DATA_32 << 16;
    DATA_32 = DATA_32 | HeightTwo; //combine both height to get a 32 bit word, more efficient.


    /* Unlock the Flash to enable the flash control register access *************/
    FLASH_Unlock();

    /* Erase the user Flash area
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);

    /* Define the number of page to be erased */
    NbrOfPage = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;

    /* Erase the FLASH pages */
    for(EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        if (FLASH_ErasePage(FLASH_USER_START_ADDR + (FLASH_PAGE_SIZE * EraseCounter))!= FLASH_COMPLETE)
        {
            SuccessCode = 1; /* Error occurred while sector erase. */
        }
    }
    /* Program the user Flash area word by word
      (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

    Address = FLASH_USER_START_ADDR;

    while (Address < FLASH_USER_END_ADDR)
    {
        if (FLASH_ProgramWord(Address, DATA_32) == FLASH_COMPLETE)
        {
            Address = Address + 4;
        }
        else
        {
            SuccessCode = 2; /* Error occurred while writing data in Flash memory. */
        }
    }

    /* Lock the Flash to disable the flash control register access (recommended
       to protect the FLASH memory against possible unwanted operation) *********/
    FLASH_Lock();

    /* Check if the programmed data is OK
    MemoryProgramStatus = 0: data programmed correctly
    MemoryProgramStatus != 0: number of words not programmed correctly ******/
    Address = FLASH_USER_START_ADDR;
    MemoryProgramStatus = PASSED;

    while (Address < FLASH_USER_END_ADDR)
    {
        Data = *(__IO uint32_t *)Address;

        if (Data != DATA_32)
        {
            SuccessCode = 3;
        }

        Address = Address + 4;
    }

    return SuccessCode; //value 0: all well. value 1: problem during clearing flash.
}											//value 2: problem during writing. value 3: flash wasnt written with the right value

//function to read from flash
uint16_t ReadFromFlash( uint8_t H1orH2 )
{
    uint16_t Height;
    uint32_t Value;

    Address = FLASH_USER_START_ADDR;
    Value = *(__IO uint32_t *)Address;

    if( H1orH2 == 1 ) //return Height One
    {
        Value = Value >> 16;
        Height = Value;
    }
    else //return Height Two
    {
        Value = Value & 0x0000FFFF;
        Height = Value;
    }

    return Height;
}


/**
  * @}
  */

/**
  * @}
  */
