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

void esp8266_init()
{
    printf("AT+CWMODE=1\r\n");
    DELAY_MS(1000);
    printf("AT+CWJAP=\"611sys\",\"611sys611\"\r\n");
    DELAY_MS(8000);
    printf("AT+CIPSTART=\"TCP\",\"192.168.1.78\",1234\r\n");
    DELAY_MS(8000);
}

void send_to_pc(char *str)
{
    int len = 0;
    while (str[len] != '#')
    {
        len++;
    }
    str[len] = '\0';
    len += 2;
    printf("AT+CIPSEND=");
    printf("%d\r\n", len);
    DELAY_MS(500);
    printf("%s\r\n", str);
}

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
    //ftm_pwm_init(FTM0, FTM_CH6, 100, 1340); //舵机初始化  FTM0

    //电机左
    // ftm_pwm_init(FTM3, FTM_CH0, 10000, 3000);
    // ftm_pwm_init(FTM3, FTM_CH1, 10000, 1340);
    //initMotorSteer();

    esp8266_init();

    while (1)
    {
        // int i = 0;
        // led(LED0, LED_ON); //LED0 亮
        // led_turn(LED1);    //LED1翻转

        // DELAY_MS(500); //延时500ms

        // led(LED0, LED_OFF); //LED0 灭
        led_turn(LED1); //LED1翻转

        send_to_pc("haha#");

        // DELAY_MS(500); //延时500ms

        //ftm_pwm_duty(FTM0, FTM_CH6, 1470);
        //DELAY_MS(100); //延时500ms
        //ftm_pwm_duty(FTM0, FTM_CH6, 1190);
        //int i = -30;
        DELAY_MS(100); //延时500ms
        // setSteer(STEER_MID + 100);
        // if (i >= 30)
        //     i = -30;

        // ftm_pwm_duty(FTM3, FTM_CH0, 1470);
        // ftm_pwm_duty(FTM3, FTM_CH1, 4470);
    }
}
