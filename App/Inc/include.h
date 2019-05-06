#ifndef __INCLUDE_H__
#define __INCLUDE_H__

#include "common.h"

/*
 * Include 用户自定义的头文件
 */
#include "MK60_wdog.h"
#include "MK60_gpio.h" //IO口操作
#include "MK60_uart.h" //串口
#include "MK60_SysTick.h"
#include "MK60_lptmr.h" //低功耗定时器(延时)
#include "MK60_pit.h"   //PIT
#include "MK60_FLASH.h" //FLASH

#include "VCAN_LED.H" //LED
#include "VCAN_KEY.H" //KEY

#include "MK60_FTM.h"

#include "motor_steer.h"       //电机、舵机
#include "uart.h"              //设置串口
#include "waitToDo.h"          //每次循环检测是否要执行的操作
#include "command.h"           //命令行操作
#include "quad.h"              //编码器
#include "VCAN_camera.h"       //摄像头
#include "VCAN_OV7725_Eagle.h" //摄像头
//#include "esp.h"         //无线模块

#endif //__INCLUDE_H__
