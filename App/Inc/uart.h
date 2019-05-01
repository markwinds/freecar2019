#ifndef __UART_H__
#define __UART_H__

#include "common.h"
#include "include.h"

extern char uart4_buff[1024]; //接收上位机的缓存
extern int esp_receive_flag;  //判断是否开始接收上位机的数据,数据以$开头，#结尾
extern int uart4_buff_size;   //已经接收到的数据的大小
extern int uart4_to_do_flag;  //处理串口中断4的标志

extern void uart4_handler(void);
extern void initUart();
extern void buffHandler();

#endif //__UART_H__