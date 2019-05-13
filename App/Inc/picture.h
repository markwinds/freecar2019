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

//存放UI参数的扇区
#define SECTOR_FOR_UI_COUNT (LAST_SECTOR_ID - PICTURE_SECTOR_SIZE - 1)

//最多存放图片的数量
#define MAX_PICTURE_NUM (PICTURE_SECTOR_SIZE * PICTURE_NUM_PER_SECTOR)

//存一幅图片要偏移的地址数
#define OFFSET_PER_PICTURE (500 / PICTURE_NUM_PER_SECTOR)

#define readPictureToLCDDefault() readPictureToDisplayer(picture_now_id, LCD)
#define readPictureToUARTDefault() readPictureToDisplayer(picture_now_id, UART)

//显示器
enum Displayer
{
    LCD,
    UART,
    buff
};

//处理图片的方法
struct DealPictureWay
{
    void (*dealPictureFunction)();
    char way_name[30];
};

extern int picture_num; //存储图片的数量
extern int picture_now_id;
extern struct DealPictureWay *now_deal_picture_way;

extern void writePictureToFlash();
extern void readPictureToDisplayer(int picture_id, enum Displayer displayer);
extern void nextPictureID();
extern void beforePictureID();
extern void writeParameterToFlash();
extern void readParameterFromFlash();
extern void deleteAllPicture();
extern void showFilterPicture();
extern void showSobelPicture();
extern void initFlashs();
extern void nextDealPictureWay();
extern void beforeDealPictureWay();
extern void showOriginalPicture();
extern void showFilterSobelPicture();
extern void writeUIParameterToFlash();
extern void readUIParameterFromFlash();

#endif