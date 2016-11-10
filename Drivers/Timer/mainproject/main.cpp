/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    23-March-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "h/counter.h"
#include "main.h"
#include "usart.h"


/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
extern volatile char rx_buffer;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
Time Anita;
USART_1 Truus;

int main(void) {
    RCC_ClocksTypeDef RCC_Clocks;


    // Configure LED3 and LED4 on STM32F0-Discovery
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    // SysTick end of count event each 1ms
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
    Truus.init();
    Anita.init(20,0);

    while (1) {

    }
}

void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {


        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        Anita.Raisetime();
        Anita.incrementTime();
        //Truus << Anita.GetSeconds();
      //  Truus << "\n";
        STM_EVAL_LEDToggle(LED3);

    }
}

void USART1_IRQHandler() {
    //Check if interrupt was because data is received
    if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
        //Do your stuff here
        Truus << "Sec:  ";
        Truus << Anita.GetSeconds();
//        Truus < USART_ReceiveData(USART1);
        Truus << "\n";

        //Clear interrupt flag
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 1 ms.
  * @retval None
  */
void delay(__IO uint32_t nTime) {
    TimingDelay = nTime;

    while (TimingDelay != 0);
}

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void) {
    if (TimingDelay != 0x00) {
        TimingDelay--;
    }
}

#ifdef __cplusplus
}
#endif

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
