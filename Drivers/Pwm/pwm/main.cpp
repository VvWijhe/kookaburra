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
#include <stm32f0xx.h>
#include "main.h"
#include "math.h"

#define CONVERSIONG 3.9

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */







int main(void){

    uint32_t comparator =0;

    pwm pwm1;
    pwm1.init_pwm();


        // Delay ~ 0.01 sec.
       // pwm1.delay(SystemCoreClock/8/100);

        // Update compare value

        comparator=40;

        TIM_SetCompare4(TIM2, comparator);

    while (1){;}
}

void pwm::init_pwm()
{
    GPIO_InitTypeDef        GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef       TIM_OCInitStructure;

    uint32_t compare=0;

    // Initialize User Button on STM32F0-Discovery
    STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

    //[..] To use the Timer in Output Compare mode, the following steps are
    //     mandatory:

    //(#) Enable TIM clock using
    //    RCC_APBxPeriphClockCmd(RCC_APBxPeriph_TIMx, ENABLE) function.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //(#) Configure the TIM pins by configuring the corresponding GPIO pins
    //    This is LED3 on STM32F0-Discovery
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_1);

    //(#) Configure the Time base unit as described in the first part of this
    //    driver, if needed, else the Timer will run with the default
    //    configuration:
    //    (++) Autoreload value = 0xFFFF.
    //    (++) Prescaler value = 0x0000.
    //    (++) Counter mode = Up counting.
    //    (++) Clock Division = TIM_CKD_DIV1.
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)((SystemCoreClock / 5000) - 1);
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //(#) Fill the TIM_OCInitStruct with the desired parameters including:
    //    (++) The TIM Output Compare mode: TIM_OCMode.
    //    (++) TIM Output State: TIM_OutputState.
    //    (++) TIM Pulse value: TIM_Pulse.
    //    (++) TIM Output Compare Polarity : TIM_OCPolarity.
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = compare;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    //(#) Call TIM_OCxInit(TIMx, &TIM_OCInitStruct) to configure the desired
    //    channel with the corresponding configuration.
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    //(#) Call the TIM_Cmd(ENABLE) function to enable the TIM counter.
    TIM_Cmd(TIM2, ENABLE);

}

void pwm::delay (const int d){
    volatile int i;

    for (i=d; i> 0; i--){
        ;
    }
    return;
}