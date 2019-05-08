#include "common.h"
#include "include.h"

void main()
{
    led_init(LED0); //初始化LED0
    led_init(LED1); //初始化LED1
    led_init(LED2);
    led_init(LED3);
    // initUart();
    // initMotorSteer();
    Quad_Init();
    LCD_init();
    camera_init(imgbuff);

    Site_t site = {0, 0};   //x = 10 ,y = 20
    Size_t size = {80, 60}; //W = 320,H = 240

    while (1)
    {
        led_turn(LED1);
        DELAY_MS(20);
        camera_get_img();
        LCD_Img_Binary(site, size, imgbuff);
        // setSpeedRight(0);
        // DELAY_MS(1000);

        // setSteer(-50);
        // setSpeedLeft(3000);
        // DELAY_MS(1000);

        // setSteer(50);
        // setSpeedRight(3000);
        // DELAY_MS(1000);

        // setSteer(0);
        // setSpeedLeft(0);
        // DELAY_MS(1000);

        // camera_get_img();
    }
}
