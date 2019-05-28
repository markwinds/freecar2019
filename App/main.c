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
	//initMotorSteer();
	Quad_Init();
	LCD_init();
	camera_init(imgbuff);

	while (1)
	{
		// setSteer(-50);
		// DELAY_MS(2000);
		// setSteer(0);
		// DELAY_MS(2000);
		// setSteer(50);
		// DELAY_MS(2000);
		// if (p_current_state == &imgbuff_show)
		// {
		led_turn(LED0);
		setSpeedLeft(test_speed + speed_different);
		setSpeedRight(test_speed);
		// }
		// else
		// {
		// 	setSpeedLeft(0);
		// 	setSpeedRight(0);
		// }

		led_turn(LED1);
		DELAY_MS(20);
		waitToDo();
	}
}
