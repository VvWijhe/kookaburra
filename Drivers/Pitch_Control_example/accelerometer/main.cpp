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
#include <mpu6050.h>
#include <pwm.h>
#include "main.h"
#include "mpu6050.h"
#include "usart.h"
#include "math.h"

#define CONVERSIONG 3.9

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

PWM pwm;
USART_1 usart;
MPU6050 accelerometer;
accelGyroDataRaw_t accelGyroDataRaw;
double pitch, roll, yaw;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void) {
    RCC_ClocksTypeDef RCC_Clocks;

    // Configure LED3 and LED4 on STM32F0-Discovery
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    // SysTick end of count event each 1ms
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

    // Initialize MPU6050
    // Power ON, Clock source X Gyro, Highest sensivity
    accelerometer.init();

    //initialize usart
    usart.init();

    // Init pwm
    pwm.initServo();

    usart << "This is a test application for the pwm\n";

    while (1) {
        if (accelerometer.testConnection()) {
            STM_EVAL_LEDToggle(LED3);
        } else {
            STM_EVAL_LEDToggle(LED4);
        }

        accelerometer.getRawAccelGyro(&accelGyroDataRaw);

        double accelerationX = (accelGyroDataRaw.Ax * CONVERSIONG);
        double accelerationY = (accelGyroDataRaw.Ay * CONVERSIONG);
        double accelerationZ = (accelGyroDataRaw.Az * CONVERSIONG);

        pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/M_PI;
        roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;
        yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/M_PI;

        pitch += 90;

        if(pitch > 0 && pitch < 180) {
            pwm.cycle(2, (uint32_t)pitch);
        }

        usart << "Pitch: ";
        usart << pitch;
        usart << "\nRoll: ";
        usart << roll;
        usart << "\n\n";
        delay(50);
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
