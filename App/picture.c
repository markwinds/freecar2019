#include "common.h"
#include "include.h"

int sector_id = LAST_SECTOR_ID;   //扇区的id
int sector_offset = 0;			  //扇区的偏移地址
int picture_num = 0;			  //存储图片的数量
int picture_now_id = 0;			  //当前的图片id,id从0开始
int picture_write_id = 0;		  //写入图片时，为图片分配的id,与picture_num一起被写入flash
uint8 flash_imgbuff[CAMERA_SIZE]; //因为imgbuff是摄像头会直接操作的，所以这里重新建一个缓存区用于暂存数据
uint8 flash_img[CAMERA_SIZE * 8]; //暂存解压图像的数据

struct DealPictureWay deal_picture_way[] = {
	{showOriginalPicture, "Origin"},
	{showFilterPicture, "Filted"},
	{showSobelPicture, "Sobeled"},
	{showFilterSobelPicture, "F and S"},
	{NULL, "end"}};

struct DealPictureWay *now_deal_picture_way = deal_picture_way;

/*******************************读写flash的参数定位*******************************************************/
/*
利用图片的ID找到扇区id和offset的值
*/
void updateIDFromPictureID(int picture_id)
{
	sector_id = LAST_SECTOR_ID - picture_id / PICTURE_NUM_PER_SECTOR;			//得到扇区编号
	sector_offset = (picture_id % PICTURE_NUM_PER_SECTOR) * OFFSET_PER_PICTURE; //得到偏移地址
}

void nextPictureID()
{
	picture_now_id = (picture_now_id + 1) % picture_num;
}

void beforePictureID()
{
	picture_now_id = (picture_now_id - 1 + picture_num) % picture_num;
}

void deleteAllPicture()
{
	picture_num = 0;
	picture_now_id = 0;
	picture_write_id = 0;
	writeParameterToFlash();
}

/*******************************串口输出*******************************************************/

/*
将flash_imgbuff的图片从串口输出
*/
void UARTSendPicture(uint8 *src)
{
	img_extract(img, src, CAMERA_SIZE);
	uint8 *p = img;
	//输出不分割的完整图形
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			if (*(p++) == 0)
				printf("0");
			else
				printf("1");
		}
		printf("\n");
	}
	printf("\n\n\n\n");
	//输出带间隔的图像，方便看图像的时候数像素点
	for (int i = 0; i < 60; i++)
	{
		if (i % 10 == 0)
			printf("\n");
		for (int j = 0; j < 80; j++)
		{
			if (*(p++) == 0)
				printf("0");
			else
				printf("1");
			if (j % 10 == 0)
				printf(" ");
		}
		printf("\n");
	}
}

/***********************************从flash读写图片*****************************************************/

/*
将imgbuff中的数据写入到flash中
*/
void writePictureToFlash()
{
	updateIDFromPictureID(picture_write_id);
	if (0 == sector_offset)
		flash_erase_sector(sector_id);
	int write_num = 300 / PICTURE_NUM_PER_SECTOR; //一张图片需要写几次
	for (int i = 0; i < write_num; i++)			  //写入图片数据
	{
		FLASH_WRITE_TYPE data;
#if defined(MK60DZ10)
		data = ((uint32)imgbuff[4 * i] << 24) +
			   ((uint32)imgbuff[4 * i + 1] << 16) +
			   ((uint32)imgbuff[4 * i + 2] << 8) +
			   ((uint32)imgbuff[4 * i + 3]);
		flash_write(sector_id, sector_offset * 4, data);
#elif defined(MK60F15)
		data = ((uint64)imgbuff[8 * i] << 56) +
			   ((uint64)imgbuff[8 * i + 1] << 48) +
			   ((uint64)imgbuff[8 * i + 2] << 40) +
			   ((uint64)imgbuff[8 * i + 3] << 32) +
			   ((uint64)imgbuff[8 * i + 4] << 24) +
			   ((uint64)imgbuff[8 * i + 5] << 16) +
			   ((uint64)imgbuff[8 * i + 6] << 8) +
			   ((uint64)imgbuff[8 * i + 7]);
		flash_write(sector_id, sector_offset * 8, data);
#endif
		sector_offset++;
	}
	led_turn(LED0);
	DELAY_MS(200);
	led_turn(LED0);
	picture_write_id = (picture_write_id + 1) % MAX_PICTURE_NUM; //得到下一张图片应该分配的ID
	picture_num++;
}

/*
将flash_buff中的数据写入到flash
*/
void writeBuffToFlash(int location_id)
{
	updateIDFromPictureID(location_id);
	if (0 == sector_offset)
		flash_erase_sector(sector_id);
	int write_num = 300 / PICTURE_NUM_PER_SECTOR; //一张图片需要写几次
	for (int i = 0; i < write_num; i++)			  //写入图片数据
	{
		FLASH_WRITE_TYPE data;
#if defined(MK60DZ10)
		data = ((uint32)flash_imgbuff[4 * i] << 24) +
			   ((uint32)flash_imgbuff[4 * i + 1] << 16) +
			   ((uint32)flash_imgbuff[4 * i + 2] << 8) +
			   ((uint32)flash_imgbuff[4 * i + 3]);
		flash_write(sector_id, sector_offset * 4, data);
#elif defined(MK60F15)
		data = ((uint64)flash_imgbuff[8 * i] << 56) +
			   ((uint64)flash_imgbuff[8 * i + 1] << 48) +
			   ((uint64)flash_imgbuff[8 * i + 2] << 40) +
			   ((uint64)flash_imgbuff[8 * i + 3] << 32) +
			   ((uint64)flash_imgbuff[8 * i + 4] << 24) +
			   ((uint64)flash_imgbuff[8 * i + 5] << 16) +
			   ((uint64)flash_imgbuff[8 * i + 6] << 8) +
			   ((uint64)flash_imgbuff[8 * i + 7]);
		flash_write(sector_id, sector_offset * 8, data);
#endif
		sector_offset++;
	}
}

/*
从flash读取图片到显示器，显示器可以是LCD或者UART或者flash_buff
*/
void readPictureToDisplayer(int picture_id, enum Displayer displayer)
{
	updateIDFromPictureID(picture_id);
	int read_num = 300 / PICTURE_NUM_PER_SECTOR; //一张图片需要读几次
	for (int i = 0; i < read_num; i++)			 //写入图片数据
	{
		FLASH_WRITE_TYPE data;
#if defined(MK60DZ10)
		data = flash_read(sector_id, sector_offset * 4, uint32);
		flash_imgbuff[4 * i] = data >> 24;
		flash_imgbuff[4 * i + 1] = (data & 0x00ff0000) >> 16;
		flash_imgbuff[4 * i + 2] = (data & 0x0000ff00) >> 8;
		flash_imgbuff[4 * i + 3] = (data & 0x000000ff);
#elif defined(MK60F15)
		data = flash_read(sector_id, sector_offset * 8, uint64);
		flash_imgbuff[8 * i] = data >> 56;
		flash_imgbuff[8 * i + 1] = (data & 0x00ff000000000000) >> 48;
		flash_imgbuff[8 * i + 2] = (data & 0x0000ff0000000000) >> 40;
		flash_imgbuff[8 * i + 3] = (data & 0x000000ff00000000) >> 32;
		flash_imgbuff[8 * i + 4] = (data & 0x00000000ff000000) >> 24;
		flash_imgbuff[8 * i + 5] = (data & 0x0000000000ff0000) >> 16;
		flash_imgbuff[8 * i + 6] = (data & 0x000000000000ff00) >> 8;
		flash_imgbuff[8 * i + 7] = (data & 0x00000000000000ff);
#endif
		sector_offset++;
	}
	if (displayer == LCD)
	{
		LCDShowPicture(flash_imgbuff);
	}
	else if (displayer == UART)
	{
		UARTSendPicture(flash_imgbuff);
	}
	else if (displayer == buff)
	{
	}
}

/***********************************从flash读写参数*****************************************************/

void writeParameterToFlash()
{
	flash_erase_sector(SECTOR_FOR_PICTURE_COUNT);
	flash_write(SECTOR_FOR_PICTURE_COUNT, 0 * PICTURE_NUM_PER_SECTOR * 2, (FLASH_WRITE_TYPE)picture_num);
	flash_write(SECTOR_FOR_PICTURE_COUNT, 1 * PICTURE_NUM_PER_SECTOR * 2, (FLASH_WRITE_TYPE)picture_write_id);
	flash_write(SECTOR_FOR_PICTURE_COUNT, 2 * PICTURE_NUM_PER_SECTOR * 2, (FLASH_WRITE_TYPE)picture_now_id);
}

void readParameterFromFlash()
{
	picture_num = (int)flash_read(SECTOR_FOR_PICTURE_COUNT, 0 * PICTURE_NUM_PER_SECTOR * 2, FLASH_WRITE_TYPE);
	picture_write_id = (int)flash_read(SECTOR_FOR_PICTURE_COUNT, 1 * PICTURE_NUM_PER_SECTOR * 2, FLASH_WRITE_TYPE);
	picture_now_id = (int)flash_read(SECTOR_FOR_PICTURE_COUNT, 2 * PICTURE_NUM_PER_SECTOR * 2, FLASH_WRITE_TYPE);
}

/***********************************显示各种算法处理后的图片*****************************************************/

void showOriginalPicture()
{
	LCD_clear(WHITE);
	readPictureToDisplayer(picture_now_id, buff);
	LCDShowPicture(flash_imgbuff);
}

void showFilterPicture()
{
	LCD_clear(WHITE);
	readPictureToDisplayer(picture_now_id, buff);
	img_extract(flash_img, flash_imgbuff, CAMERA_SIZE);
	filter(img, flash_img);
	img_compress(img, flash_imgbuff, CAMERA_SIZE);
	LCDShowPicture(flash_imgbuff);
}

void showSobelPicture()
{
	LCD_clear(WHITE);
	readPictureToDisplayer(picture_now_id, buff);
	img_extract(flash_img, flash_imgbuff, CAMERA_SIZE);
	sobel(img, flash_img);
	img_compress(img, flash_imgbuff, CAMERA_SIZE);
	LCDShowPicture(flash_imgbuff);
}

void showFilterSobelPicture()
{
	LCD_clear(WHITE);
	readPictureToDisplayer(picture_now_id, buff);
	img_extract(flash_img, flash_imgbuff, CAMERA_SIZE);
	filter(img, flash_img);
	sobel(flash_img, img);
	img_compress(flash_img, flash_imgbuff, CAMERA_SIZE);
	LCDShowPicture(flash_imgbuff);
}

/***********************************flash初始化*****************************************************/

/*
初始化flash，从flash中读取存储的参数信息，并激活图片的读写操作
*/
void initFlashs()
{
	readParameterFromFlash();
	int reflash_num = picture_write_id % PICTURE_NUM_PER_SECTOR;
	if (reflash_num)
	{
		for (int i = 0; i < reflash_num; i++)
		{
			readPictureToDisplayer(picture_write_id - reflash_num + i, buff);
			writeBuffToFlash(picture_write_id - reflash_num + i + 4);
		}
		for (int i = 0; i < reflash_num; i++)
		{
			readPictureToDisplayer(picture_write_id - reflash_num + i + 4, buff);
			writeBuffToFlash(picture_write_id - reflash_num + i);
		}
	}
}

/***********************************处理图片函数定位*****************************************************/
void nextDealPictureWay()
{
	now_deal_picture_way++;
	if (0 == strcmp(now_deal_picture_way->way_name, "end"))
	{
		now_deal_picture_way = deal_picture_way;
	}
	now_deal_picture_way->dealPictureFunction();
	Site_t site = {40, 80};
	LCD_str(site, (uint8 *)(now_deal_picture_way->way_name), BLACK, WHITE);
}

void beforeDealPictureWay()
{
	if (now_deal_picture_way == deal_picture_way)
	{
		while (strcmp(now_deal_picture_way->way_name, "end") != 0)
		{
			now_deal_picture_way++;
		}
	}
	now_deal_picture_way--;
	now_deal_picture_way->dealPictureFunction();
	Site_t site = {40, 80};
	LCD_str(site, (uint8 *)(now_deal_picture_way->way_name), BLACK, WHITE);
}

/***********************************UI输入参数的flash操作*****************************************************/
void writeUIParameterToFlash()
{
	int i = 0;
	flash_erase_sector(SECTOR_FOR_UI_COUNT); //擦除扇区,擦一次只能写一次
	while (strcmp(screen_data[i].data_name, "end") != 0)
	{
		if (screen_data[i].ip >= 0)
		{
			flash_write(SECTOR_FOR_UI_COUNT, screen_data[i].ip * PICTURE_NUM_PER_SECTOR * 2,
						(FLASH_WRITE_TYPE)(*(screen_data[i].data_value)));
		}
		i++;
	}
}

void readUIParameterFromFlash()
{
	int i = 0;
	while (strcmp(screen_data[i].data_name, "end") != 0)
	{
		if (screen_data[i].ip >= 0)
		{
			*(screen_data[i].data_value) = (int)flash_read(SECTOR_FOR_UI_COUNT,
														   screen_data[i].ip * PICTURE_NUM_PER_SECTOR * 2,
														   FLASH_WRITE_TYPE);
		}
		i++;
	}
}
