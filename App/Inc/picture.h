#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "include.h"

//如果要将中间行信息写入flash，则需要宏定义
//#define FLASH_MIDDLE_LINE
#define PICTURE_SECTOR_SIZE 10
//定义每个扇区存放的图片数量
#define PICTURE_NUM_PER_SECTOR (FLASH_SECTOR_SIZE / 1024)
//存放picture_count的扇区
#define SECTOR_FOR_PICTURE_COUNT (SECTOR_NUM - 1 - PICTURE_SECTOR_SIZE)

// extern int sector_id;      //记录写入picture时的扇区
// extern int sector_offset;  //记录写入picture时的偏移地址
// extern uint8 imgbuff1[CAMERA_SIZE];

// extern void flash_Picture();
// extern void save_Picture();
// extern void delete_Picture();
// extern void read_Picture_Array();
// extern void next_Write_Location(int f);
// extern int before_Write_Location();
// extern void send_Picture10();
// extern int nextReadLocation();

#endif