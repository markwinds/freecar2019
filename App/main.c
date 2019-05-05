/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       main.c
 * @brief      山外K60 平台主程序
 * @author     山外科技
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"

/*!
 *  @brief      main函数
 *  @since      v5.0
 *  @note       测试 LED 功能是否正常
                看到的效果是LED0和LED1同时亮灭闪烁
 */
void main()
{
    led_init(LED0); //初始化LED0
    led_init(LED1); //初始化LED1
    initUart();
    initMotorSteer();
    Quad_Init();

    // strtok(NULL, " ");
    // vector<int> a;
    while (1)
    {
        led_turn(LED0); //LED1翻转
        setSpeedRight(0);
        DELAY_MS(1000);

        setSteer(-50);
        setSpeedLeft(3000);
        DELAY_MS(1000);

        setSteer(50);
        setSpeedRight(3000);
        DELAY_MS(1000);

        setSteer(0);
        setSpeedLeft(0);
        DELAY_MS(1000);
        }
}
