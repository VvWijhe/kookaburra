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
#include "MS5611.h"
#include "main.h"
#include "usart.h"

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

USART_1 usart;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {
    RCC_ClocksTypeDef RCC_Clocks;
    MS5611 Barometer;


    // Configure LED3 and LED4 on STM32F0-Discovery
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    // SysTick end of count event each 1ms
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

    // Initialize MS5611
    Barometer.initialize();

    //initialize usart
    usart.init();

    usart << "This is a test application for the barometer\n";

    while (1) {
        usart << "Pressure: ";
        usart << Barometer.getPressure();
        usart << "\n";

        usart << "Temperature: ";
        usart << Barometer.getTemperature();
        usart << "\n";

        usart << "Altitude: ";
        usart << Barometer.getAltitude();
        usart << "\n\n";

        delay(100);
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
