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
#include <pwm.h>
#include "main.h"

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

USART_1 tryout;
PWM pwm1;

int main(void){
    char c[2];
//    int digit = 33;

    tryout.init();
    pwm1.initServo();
    pwm1.initMotor();

    while(1){
        // Recive data
        tryout << "Enter three digits:\n";
        tryout > c[0];
        tryout > c[1];
        tryout > c[2];
        tryout << "numer is: ";
        tryout << c;
        tryout << "\n";


        // Convert char to int and send to servo
        pwm1.cycle(2, (uint16_t(atoi(c))));
    }
}

