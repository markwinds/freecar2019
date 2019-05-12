#ifndef _PICTURE_H_
#define _PICTURE_H_

#include "include.h"

//如果要将中间行信息写入flash，则需要宏定义
//#define FLASH_MIDDLE_LINE

//可以存放图片的扇区数量
#define PICTURE_SECTOR_SIZE 10

//每个扇区可以存放图片的数量
#define PICTURE_NUM_PER_SECTOR (FLASH_SECTOR_SIZE / 1024)

//最后一个用来存放图片的扇区
#define LAST_SECTOR_ID SECTOR_NUM

//存放picture_count的扇区
#define SECTOR_FOR_PICTURE_COUNT (LAST_SECTOR_ID - PICTURE_SECTOR_SIZE)

//最多存放图片的数量
#define MAX_PICTURE_NUM (PICTURE_SECTOR_SIZE * PICTURE_NUM_PER_SECTOR)

//存一幅图片要偏移的地址数
#define OFFSET_PER_PICTURE (500 / PICTURE_NUM_PER_SECTOR)

extern int picture_num;    //存储图片的数量
extern int picture_now_id; //当前的图片id,id从1开始

extern void writePictureToFlash();
extern void readPictureToLCD(int picture_id);

// extern int sector_id;
// extern int sector_offset;
// extern int picture_num;    //存储图片的数量
// extern int picture_now_id; //当前的图片id,id从1开始
// extern uint8 imgbuff1[CAMERA_SIZE];

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