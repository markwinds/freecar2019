#ifndef __ESP_H__
#define __ESP_H__

#include "common.h"
#include "include.h"

enum ESP_MODE
{
    network = 1,
    direct,
    network_and_direct
};

//上机电脑的IP地址
#define PC_IP "192.168.1.78"

//配置esp的工作模式
extern enum ESP_MODE esp_mode;

extern void esp8266_init();

#endif //__ESP_H__