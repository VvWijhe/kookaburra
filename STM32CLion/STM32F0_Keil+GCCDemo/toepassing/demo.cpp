
#include <stm32f0basis.h>
#include <cpalDriver.h>

int main(void)
{
	wachtklokInit();
	
	LEDDriver leds;
	
	cpalInit();  /* init de i2c cpal bibliotheek */

	leds.schakel(LEDDriver::LED3,true);
	leds.schakel(LEDDriver::LED4,true);
	wachtMilliSecs(1000);
	leds.schakel(LEDDriver::LED4,false);
	
	do
	{
		leds.om(LEDDriver::LED3);
		leds.om(LEDDriver::LED4);
		
		/* wacht hier */
		wachtMilliSecs(500);
	} while (1==1);


}
