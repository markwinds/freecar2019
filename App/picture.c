#include "include.h"
#include "picture.h"

int sector_id = SECTOR_NUM - 1;
int sector_offset = 0;
int picture_count = 0;  //存储图片的数量
int picture_now_id = 0; //当前的图片id,id从1开始
uint8 imgbuff1[CAMERA_SIZE];

// void save_Picture()
// {
// 	if (1 == save_picture)
// 	{
// 		flash_Picture();
// 		save_picture = 0; //清空标志位
// 	}
// }

// void flash_Picture() //将imgbuff图片的信息写入flash
// {
// 	int i = 0;
// 	uint32 data;

// 	sector_offset = 0;
// 	sector_id = SECTOR_NUM - 1;
// 	/*找到可以存放数据的扇区*/
// 	while (813 == flash_read(sector_id, sector_offset * 4, uint32)) //在每组图片信息的头部flash有标志是否已经存储图片的标志位，存储图片时标志位是813
// 	{
// 		next_Write_Location();
// 	}
// 	/*写入对应的数据*/
// 	if (0 == sector_offset) //如果是一个新的扇区
// 	{
// 		flash_erase_sector(sector_id); //擦除该扇区，也就是扇区初始化
// 	}
// 	for (i = -1; i < 150; i++) //图片数据
// 	{
// 		if (-1 == i) //写入头标志
// 		{
// 			flash_write(sector_id, sector_offset * 4, 813);
// 			sector_offset++;
// 			continue;
// 		}
// 		data = ((uint32)imgbuff[4 * i] << 24) + ((uint32)imgbuff[4 * i + 1] << 16) + ((uint32)imgbuff[4 * i + 2] << 8) + ((uint32)imgbuff[4 * i + 3]);
// 		flash_write(sector_id, sector_offset * 4, data);
// 		sector_offset++;
// 	}

// #if defined(FLASH_MIDDLE_LINE)
// 	for (i = 0; i < 15; i++)
// 	{
// 		data = ((uint32)middleline[4 * i] << 24) + ((uint32)middleline[4 * i + 1] << 16) + ((uint32)middleline[4 * i + 2] << 8) + ((uint32)middleline[4 * i + 3]);
// 		flash_write(sector_id, sector_offset * 4, data);
// 		sector_offset++;
// 	}
// 	for (i = 0; i < 15; i++)
// 	{
// 		data = ((uint32)left_black[4 * i] << 24) + ((uint32)left_black[4 * i + 1] << 16) + ((uint32)left_black[4 * i + 2] << 8) + ((uint32)left_black[4 * i + 3]);
// 		flash_write(sector_id, sector_offset * 4, data);
// 		sector_offset++;
// 	}
// 	for (i = 0; i < 15; i++)
// 	{
// 		data = ((uint32)right_black[4 * i] << 24) + ((uint32)right_black[4 * i + 1] << 16) + ((uint32)right_black[4 * i + 2] << 8) + ((uint32)right_black[4 * i + 3]);
// 		flash_write(sector_id, sector_offset * 4, data);
// 		sector_offset++;
// 	}
// #endif
// 	next_Write_Location();
// 	led_turn(LED0);
// 	DELAY_MS(200);
// 	led_turn(LED0);
// }

// /*
// 寻找下一个写入的位置
// */
// void next_Write_Location()
// {
// 	sector_offset = (sector_offset / 250 + 1) * 250;
// 	if (sector_offset >= 250 * PICTURE_NUM_PER_SECTOR)
// 	{
// 		sector_offset = 0;
// 		sector_id--;
// 	}
// 	if (sector_id < SECTOR_NUM - 1 - PICTURE_SECTOR_SIZE) //如果写入超过的10个扇区
// 	{
// 		sector_offset = 0;
// 		sector_id = SECTOR_NUM - 1;
// 	}
// }

// /*
// 读取图片时查找下一个位置
// 返回第几张图片
// */
// int nextReadLocation()
// {
// 	sector_offset = (sector_offset / 250 + 1) * 250;
// 	if (sector_offset >= 250 * PICTURE_NUM_PER_SECTOR)
// 	{
// 		sector_offset = 0;
// 		sector_id--;
// 	}
// 	if (flash_read(sector_id, sector_offset * 4, uint32) != 813 || sector_id < SECTOR_NUM - 1 - PICTURE_SECTOR_SIZE)
// 	{
// 		sector_offset = 0;
// 		sector_id = SECTOR_NUM - 1;
// 	}
// 	return (((SECTOR_NUM - 1) - sector_id) * PICTURE_NUM_PER_SECTOR + (sector_offset / 250) + 1);
// }

// int before_Write_Location()
// {
// 	int i = 0;
// 	int j = 0;

// 	if (sector_offset >= 250)
// 	{
// 		sector_offset = 0;
// 	}
// 	else
// 	{
// 		sector_offset = 250;
// 		sector_id++;
// 	}
// 	if (sector_id > SECTOR_NUM - 1)
// 	{
// 		sector_offset = 0;
// 		sector_id = SECTOR_NUM - 1;
// 		i = 0;
// 		while (nextReadLocation() != 1)
// 		{
// 			i++;
// 		}
// 		for (j = 0; j < i; j++)
// 		{
// 			nextReadLocation();
// 		}
// 	}
// 	return (((SECTOR_NUM - 1) - sector_id) * PICTURE_NUM_PER_SECTOR + (sector_offset / 250) + 1);
// }

// /*
// 删除存储在flash的所有图片
// */
// void delete_Picture()
// {
// 	sector_offset = 0;
// 	sector_id = SECTOR_NUM - 1;
// 	while (813 == flash_read(sector_id, sector_offset * 4, uint32))
// 	{
// 		flash_erase_sector(sector_id);
// 		sector_id--;
// 	}
// 	sector_offset = 0;
// 	sector_id = SECTOR_NUM - 1;
// }

// void read_Picture_Array()
// {
// 	int i = 0;
// 	uint32 data = 0;
// 	Site_t site = {0, 0};
// 	Size_t size;
// 	Size_t imgsize = {CAMERA_W, CAMERA_H};
// 	//size.H = LCD_H;
// 	//size.W = LCD_W;
// 	//size.H = LCD_H/2;
// 	//size.W = LCD_W/2;
// 	size.H = 60;
// 	size.W = 80;

// 	if (1 == picture_choose)
// 	{
// 		picture_choose = 10;
// 		picture_count = before_Write_Location();
// 	}
// 	else if (3 == picture_choose)
// 	{
// 		picture_choose = 10;
// 		picture_count = nextReadLocation();
// 	}
// 	else if (2 == picture_choose) //读取当前的数据
// 	{
// 		if (sector_offset >= 0 && sector_offset < 250)
// 		{
// 			sector_offset = 1;
// 		}
// 		else
// 		{
// 			sector_offset = 251;
// 		}
// 		for (i = 0; i < 150; i++) //图片数据
// 		{
// 			data = flash_read(sector_id, sector_offset * 4, uint32);
// 			imgbuff1[4 * i] = data >> 24;
// 			imgbuff1[4 * i + 1] = (data & 0x00ff0000) >> 16;
// 			imgbuff1[4 * i + 2] = (data & 0x0000ff00) >> 8;
// 			imgbuff1[4 * i + 3] = data & 0x000000ff;
// 			sector_offset++;
// 		}
// 		vcan_sendimg(imgbuff1, CAMERA_SIZE);
// 		LCD_Img_Binary_Z(site, size, imgbuff1, imgsize);
// 		sector_offset -= 150;
// 		picture_choose_before = 2;
// 		picture_choose = 0;
// 	}
// 	else if (4 == picture_choose)
// 	{
// 		if (sector_offset >= 0 && sector_offset < 250)
// 		{
// 			sector_offset = 151;
// 		}
// 		else
// 		{
// 			sector_offset = 401;
// 		}
// 		for (i = 0; i < 45; i++) //图片数据
// 		{
// 			if (0 == (i % 3) && i != 0)
// 				printf("\n");
// 			if (0 == (i % 15) && i != 0)
// 				printf("\n");
// 			if (0 == i)
// 			{
// 				printf("\n\n\n\n\n");
// 				printf("                                   Picture:%d\n", picture_count);
// 			}

// 			data = flash_read(sector_id, sector_offset * 4, uint32);
// 			printf("%5d ", (int8)((data >> 24)));
// 			printf("%5d ", (int8)((data & 0x00ff0000) >> 16));
// 			printf("%5d ", (int8)((data & 0x0000ff00) >> 8));
// 			printf("%5d ", (int8)(data & 0x000000ff));
// 			sector_offset++;
// 		}
// 		sector_offset -= 45;
// 		picture_choose = 0;
// 	}
// 	else if (10 == picture_choose)
// 	{
// 		if (sector_offset >= 0 && sector_offset < 250)
// 		{
// 			sector_offset = 1;
// 		}
// 		else
// 		{
// 			sector_offset = 251;
// 		}
// 		for (i = 0; i < 150; i++) //图片数据
// 		{
// 			data = flash_read(sector_id, sector_offset * 4, uint32);
// 			imgbuff1[4 * i] = data >> 24;
// 			imgbuff1[4 * i + 1] = (data & 0x00ff0000) >> 16;
// 			imgbuff1[4 * i + 2] = (data & 0x0000ff00) >> 8;
// 			imgbuff1[4 * i + 3] = data & 0x000000ff;
// 			sector_offset++;
// 		}
// 		send_Picture10();
// 		LCD_Img_Binary_Z(site, size, imgbuff1, imgsize);
// 		sector_offset -= 150;
// 		picture_choose_before = 10;
// 		picture_choose = 0;
// 	}
// 	else if (11 == picture_choose)
// 	{
// 		picture_count = 1;
// 		picture_choose = 10;
// 		read_Picture_Array();
// 		picture_choose = 4;
// 		read_Picture_Array();
// 		printf("\n\n\n---------------------------------------------------------------------------------------------\n\n\n");
// 		while (nextReadLocation() != 1)
// 		{
// 			picture_count++;
// 			picture_choose = 10;
// 			read_Picture_Array();
// 			picture_choose = 4;
// 			read_Picture_Array();
// 			printf("\n\n\n---------------------------------------------------------------------------------------------\n\n\n");
// 		}
// 		picture_count = 1;
// 		picture_choose = 0;
// 		sector_offset = 0;
// 		sector_id = SECTOR_NUM - 1;
// 	}
// }

// /*
// 向串口发送存在imgbuff1中的图像
// */
// void send_Picture10()
// {
// 	int i = 0;
// 	int j = 0;
// 	img_extract(img, imgbuff1, CAMERA_SIZE); //解压图像
// 	printf("\n\n                                   Picture:%d\n", picture_count);
// 	for (i = 0; i < 60; i++)
// 	{
// 		for (j = 0; j < 80; j++)
// 		{
// 			printf("%d", img[i][j]);
// 		}
// 		printf("\n");
// 	}
// 	printf("\n\n                                   Picture:%d\n", picture_count);
// 	for (i = 0; i < 60; i++)
// 	{
// 		for (j = 0; j < 80; j++)
// 		{
// 			printf("%d", img[i][j]);
// 			if (0 == ((j + 1) % 20))
// 				printf(" ");
// 		}
// 		printf("\n");
// 		if (0 == ((i + 1) % 10))
// 			printf("\n");
// 	}
// }