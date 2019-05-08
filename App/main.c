#include "common.h"
#include "include.h"

void main()
{
    initNVIC();
    led_init(LED0); //初始化LED0
    led_init(LED1); //初始化LED1
    led_init(LED2);
    led_init(LED3);
    UI_INIT();
    // initUart();
    // initMotorSteer();
    Quad_Init();
    LCD_init();
    camera_init(imgbuff);

    while (1)
    {
        led_turn(LED1);
        DELAY_MS(500);
        Open_UI();
    }
}
