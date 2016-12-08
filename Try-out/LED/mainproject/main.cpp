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
#include "counter.h"
#include "main.h"
#include "usart.h"
#include "MS5611.h"


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
Time Timer;
//USART_1 Usart;
MS5611 Barometer(MS5611_ADDRESS_CSB_LOW);
int Switch;
float Alt1;
float Alt2;
int Time1;
int Time2;
float AltSpeed;
int WantedSpeed = 5;
int WantedAlt = 5;
int main(void) {

    RCC_ClocksTypeDef RCC_Clocks;

    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDInit(LED3);

    // GPIOC Periph clock enable
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // PC7 and PC8 and PC9 in output mode
    GPIOC->MODER |= (GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0) ;
    // Push pull mode selected
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_7 | GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9) ;
    // Maximum speed setting
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR7 | GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9);
    // Pull-up and pull-down resistors disabled
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR7 | GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);


    // SysTick end of count event each 1ms
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
   // Usart.init();
    Barometer.initialize();
    Timer.init_Tim3(1);
    Timer.init_Tim16(1);

    while (1) {
//        if(Alt1 < WantedAlt+3 && Alt1 >WantedAlt-3){
//           // Color->Green;
//        }
//        if(Alt1 > WantedAlt+3){
//           // Color->Orange;
//        }
//        if(Alt1 < WantedAlt-3){
//           // Color->Red;
//        }
//
    }
}

extern "C" void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        Time2 = Time1;
        Time1 = Timer.GetSeconds();
        Alt2 = Alt1;
        Alt1 = Barometer.getAltitude();
        AltSpeed = CalculateSpeed(Alt1, Alt2, Time1, Time2);
        AltSpeed = 20;
        Timer.SetFreq16(AltSpeed);
        STM_EVAL_LEDToggle(LED4);
    }
}

extern "C" void TIM16_IRQHandler(void) {
    if(TIM_GetITStatus(TIM16, TIM_IT_Update) != RESET){
        TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
        STM_EVAL_LEDToggle(LED3);
    }
}

//void USART1_IRQHandler() {
//    //Check if interrupt was because data is received
//    if (USART_GetITStatus(USART1, USART_IT_RXNE)) {
//        //Do your stuff here
//        Usart << "Sec:  ";
//        Usart << Timer.GetSeconds();
//        Usart < USART_ReceiveData(USART1);
//        Usart << "\n";
//
//        //Clear interrupt flag
//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//    }
//}

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

float CalculateSpeed(float Altitude1, float Altitude2, int Timeq1, int Timeq2){
    float Speed = (Altitude2 - Altitude1)/(Timeq2 - Timeq1);
    return Speed;

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
