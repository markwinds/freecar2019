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
	// initMotorSteer();
	Quad_Init();
	LCD_init();
	camera_init(imgbuff);

	while (1)
	{
		led_turn(LED1);
		DELAY_MS(20);
		waitToDo();
	}
}
