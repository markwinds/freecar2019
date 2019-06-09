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
	initJudgeRoad();

	while (1)
	{
		//DELAY_MS(2000);
		waitToDo();

		if (0)
		{
			setSteer(0);
		}
		else
		{
			int32 error = getSteerPwmFromADCError();
			setSteer(error);
			setSpeedLeft(test_speed);
			setSpeedRight(test_speed);
		}

		// showADCVaule();

		//getErrorFromADC();
	}
}
