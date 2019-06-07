#include "common.h"
#include "include.h"

void main()
{
	initNVIC();
	led_init(LED0);
	led_init(LED1);
	led_init(LED2);
	led_init(LED3);
	UI_INIT();
	initUart();
	initMotorSteer();
	Quad_Init();
	LCD_init();
	//camera_init(imgbuff);
	initADC();
	initBuzzer();

	while (1)
	{

		// setSpeedLeft(test_speed + speed_different);
		//setSpeedRight(test_speed);
		DELAY_MS(2000);
		tellMeRoadType(circle);
		waitToDo();

		// int32 error = getSteerPwmFromADCError();
		// setSteer(error);
		// setSpeedLeft(test_speed);
		// setSpeedRight(test_speed);

		// showADCVaule();

		//getErrorFromADC();
	}
}
