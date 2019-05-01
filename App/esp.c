#include "common.h"
#include "include.h"

enum ESP_MODE esp_mode = network;

//初始化无线模块
void esp8266_init()
{
    uart_init(UART4, 115200);
    printf("AT+CWMODE=%d\r\n", esp_mode);
    DELAY_MS(100);
    printf("AT+CWJAP=\"sss\",\"markwinds114\"\r\n");
    DELAY_MS(5000);
    printf("AT+CIPSTART=\"TCP\",\"%s\",1234\r\n", PC_IP);
    DELAY_MS(3000);
    printf("AT+CIPMODE=1\r\n"); //开启透传模式
    DELAY_MS(10);
    printf("AT+CIPSEND\r\n"); //进入透传模式
    DELAY_MS(10);
    //printf("+++") 退出透传模式
}

//设置esp为开机透传模式
void setEsp()
{
    printf("AT+CWMODE=3"); //设置模式3，可以当做客户端发送透传，同时作为热点供电脑连接
    printf("AT+RST");      //重启模块
    DELAY_MS(1000);
    printf("AT+CWSAP=\"esp\",\"123456789\",5,4\r\n"); //设置esp模块的wifi名为esp,密码123456789
    DELAY_MS(3000);
    //此时用电脑连接这个wifi，并用网络调试工具建立tcp服务，端口为1234
    printf("AT+SAVETRANSLINK=1,\"192.168.4.2\",1234,\"TCP\"");
    //重启esp模块，打开电脑tcp，就能完成透传
}