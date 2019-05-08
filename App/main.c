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
    camera_init(imgbuff);

    // strtok(NULL, " ");
    // vector<int> a;
    while (1)
    {
        led_turn(LED1);
        DELAY_MS(500);
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
