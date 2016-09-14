/**

  ******************************************************************************  
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef F4DEMO_LEDS_H
#define F4DEMO_LEDS_H

/* Includes ------------------------------------------------------------------*/
#include <stm32Controller.h>

class LEDController : public STM32Controller {
public:

    LEDController();

    void demo();

    void tim3Demo();

    void externDemo();

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
    static uint16_t capture;
    static const __IO uint16_t
    CCR1_Val;
    static const __IO uint16_t
    CCR2_Val;
    static const __IO uint16_t
    CCR3_Val;
    static const __IO uint16_t
    CCR4_Val;
    static uint16_t PrescalerValue;

private:
    STM32I2C i2c;

    void tim3Init();

    void externInit();
};


#ifdef __cplusplus
extern "C" {
#endif

void TimingDelay_Decrement(void);

#ifdef __cplusplus
}
#endif

#endif /* F4DEMO_LEDS_H */

