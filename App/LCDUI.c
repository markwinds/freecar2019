#include "include.h"
#include <string.h>

uint8 key_on = 0;
float motor_go = 10;	//在显示状态下控制电机是否转动的变量
int colour[MAX_OPTION]; //0元素也保存有有效数据
Site_t tem_site_str[] = {0, 0, 0, 20, 0, 40, 0, 60, 0, 80, 0, 100};
Site_t tem_site_data[] = {60, 0, 60, 20, 60, 40, 60, 60, 60, 80, 60, 100};

int page = 1;		 //lcd当前所在页
int current_row = 0; //当前所在行
float flash_in = 0;  //是否写入flash

float zbt = 0;

Screen_Data screen_data[] = {
	{"M_KP", &(zbt), 1, 1},
	{"M_KI", &(zbt), 1, 2},
	{"M_KD", &(zbt), 1, 3},

	{"enM", &(zbt), 1, 0}, //使能电机
	{"speed", &(zbt), 1, 4},

	{"length", &(zbt), 1, 0},

	{"flash", &(zbt), 1, -1},
	{"de_pic", &(zbt), 1, 0},
	{"se_pic", &(zbt), 1, 0},

	{"S_KP", &(zbt), 1, 5},
	{"S_KD", &(zbt), 1, 6},

	{"end", &(zbt), 1202, 0}

};

Lcd_State *p_current_state = &imgbuff_show;

/*-----------------新增功能的函数-----------------*/
Lcd_State *quit_Lcd(Lcd_State *pThis) //退出lcd模式
{
	LCD_clear(WHITE);
	openCamera();
	return &imgbuff_show;
}

Lcd_State *goto_Begin(Lcd_State *pThis) //从等待模式进入本页第一行
{
	current_row = 1;
	colour[6 * (page - 1) + current_row - 1] = GREEN; //选中的行变成绿色
	return &normal_page;
}

Lcd_State *goto_End(Lcd_State *pThis) //从等待模式进入本页最后一行
{
	if ((colour[MAX_OPTION - 1] / 300) < (page * 6))
	{
		current_row = (colour[MAX_OPTION - 1] / 300) % 6;
	}
	else
	{
		current_row = 6;
	}
	colour[6 * (page - 1) + current_row - 1] = GREEN; //选中的行变成绿色
	return &normal_page;
}

Lcd_State *turn_Front(Lcd_State *pThis) //向前翻页
{
	if (page > 1)
	{
		page--;
	}
	return pThis;
}

Lcd_State *turn_Back(Lcd_State *pThis) //向后翻页
{
	if (page * 6 < (colour[MAX_OPTION - 1] / 300))
	{
		page++;
	}
	return pThis;
}

Lcd_State *doNothing(Lcd_State *pThis) //忽略按键操作，返回原来的状态即保持状态不变
{
	return pThis;
}

Lcd_State *goto_Set(Lcd_State *pThis) //选中参数开始设置
{
	int tempId = 6 * (page - 1) + current_row - 1;
	if (GREEN == colour[tempId])
		colour[tempId] = RED;
	else
		colour[tempId] = GREEN;
	return pThis;
}

Lcd_State *goto_next(Lcd_State *pThis) //跳转到下一个参数
{
	int tempId = 6 * (page - 1) + current_row - 1;
	if (GREEN == colour[tempId])
	{
		colour[tempId] = WHITE;
		if (!(current_row == 6 || (page - 1) * 6 + current_row == colour[MAX_OPTION - 1] / 300)) //如果不是最后一行
		{
			current_row++;
		}
		else if (page != colour[MAX_OPTION - 1] / 300 / 6 + 1) //不是最后一页
		{
			current_row = 1;
			page++;
		}
		else
		{
			return &wait_middle;
		}
		colour[6 * (page - 1) + current_row - 1] = GREEN;
		return pThis;
	}
	else
		return pThis;
}

Lcd_State *goto_Before(Lcd_State *pThis) //跳转到上一个参数
{
	if (GREEN == colour[6 * (page - 1) + current_row - 1]) //只有在未选中的情况下才进行操作
	{
		colour[6 * (page - 1) + current_row - 1] = WHITE; //将原来选项的背景色变白
		if (current_row != 1)
		{
			current_row--;
		}
		else if (page > 1) //如果不是第一页，是第一行
		{
			page--;
			current_row = 6;
		}
		else //是第一页第一行
		{
			return &wait_middle;
		}
		colour[6 * (page - 1) + current_row - 1] = GREEN;
		return pThis;
	}
	else
		return pThis;
}

/*如果在选中模式则改变数据，如果没有选中则进入等待模式*/
Lcd_State *data_Down(Lcd_State *pThis)
{
	int tempId = 6 * (page - 1) + current_row - 1;
	if (RED == colour[tempId])
	{
		if (screen_data[tempId].icrement == 99)
		{
			*(screen_data[tempId].data_value) *= -1;
		}
		else
		{
			*(screen_data[tempId].data_value) -= screen_data[tempId].icrement;
		}
		if (screen_data[tempId].ip == -1) //写flash操作
		{
			flash_In();
		}
	}
	else
	{
		colour[tempId] = WHITE;
		current_row = 0;
		return &wait_middle;
	}
	return pThis;
}

Lcd_State *data_Up(Lcd_State *pThis)
{
	int tempId = 6 * (page - 1) + current_row - 1;
	if (RED == colour[tempId])
	{
		if (screen_data[tempId].icrement == 99)
		{
			*(screen_data[tempId].data_value) *= -1;
		}
		else
		{
			*(screen_data[tempId].data_value) += screen_data[tempId].icrement;
		}
		if (screen_data[tempId].ip == -1) //写flash操作
		{
			flash_In();
		}
	}
	else
	{
		colour[tempId] = WHITE;
		current_row = 0;
		return &wait_middle;
	}
	return pThis;
}

Lcd_State *quit_show(Lcd_State *pThis) //离开摄像头显示模式
{
	closeCamera();
	LCD_clear(WHITE);
	return &wait_middle;
}

Lcd_State *go_Picture(Lcd_State *pThis)
{
	if (picture_num)
	{
		closeCamera();
		LCD_clear(WHITE);
		readPictureToLCDDefault();
		return &read_picture;
	}
	else
	{
		return pThis;
	}
}

Lcd_State *sendPictureToUART(Lcd_State *pThis) //发送当前图片到串口
{
	readPictureToUARTDefault();
	return pThis;
}

Lcd_State *read_picture_to_imgbuff_show(Lcd_State *pThis)
{
	LCD_clear(WHITE);
	openCamera();
	return &imgbuff_show;
}

Lcd_State *readBeforePicture(Lcd_State *pThis) //前一幅图片
{
	beforePictureID();
	readPictureToLCDDefault();
	return pThis;
}

Lcd_State *readNextPicture(Lcd_State *pThis)
{
	nextPictureID();
	readPictureToLCDDefault();
	return pThis;
}

Lcd_State *takePhoto(Lcd_State *pThis)
{
	writePictureToFlash();
	return pThis;
}

Lcd_State *savePicture(Lcd_State *pThis)
{
	writeParameterToFlash();
	return pThis;
}

Lcd_State *goShowDealedPicture(Lcd_State *pThis)
{
	if (picture_num)
	{
		closeCamera();
		LCD_clear(WHITE);
		readPictureToLCDDefault();
		return &show_dealed_picture;
	}
	else
	{
		return pThis;
	}
}

Lcd_State *showDealedPictureToImgbuffShow(Lcd_State *pThis)
{
	LCD_clear(WHITE);
	openCamera();
	return &imgbuff_show;
}

Lcd_State *showFilterPictures(Lcd_State *pThis)
{
	showFilterPicture();
	return pThis;
}

Lcd_State *showSobelPictures(Lcd_State *pThis)
{
	showSobelPicture();
	return pThis;
}

/*中断调用的函数*/
void onpress_M()
{
	p_current_state = p_current_state->press_M(p_current_state);
}

void onpress_U()
{
	p_current_state = p_current_state->press_U(p_current_state);
}

void onpress_D()
{
	p_current_state = p_current_state->press_D(p_current_state);
}

void onpress_L()
{
	p_current_state = p_current_state->press_L(p_current_state);
}

void onpress_R()
{
	p_current_state = p_current_state->press_R(p_current_state);
}

/*flash操作函数*/
void flash_In() //将数据写入flash
{
	int i = 0;

	flash_erase_sector(SECTOR_NUM); //擦除扇区,擦一次只能写一次
	while (strcmp(screen_data[i].data_name, "end") != 0)
	{
		if (screen_data[i].ip > 0)
		{
			flash_write(SECTOR_NUM, screen_data[i].ip * 4, (uint32)((*(screen_data[i].data_value)) * 100.0 + 0.5)); //四舍五入写入，防止float精度不够
		}
		i++;
	}
}

void flash_Out()
{
	int i = 0;
	uint32 data = 0;

	while (strcmp(screen_data[i].data_name, "end") != 0)
	{
		if (screen_data[i].ip > 0)
		{
			data = flash_read(SECTOR_NUM, screen_data[i].ip * 4, uint32);
			*(screen_data[i].data_value) = (float)((double)data / 100.0);
		}
		i++;
	}
}

void PORTD_IRQHandler()
{
	uint32 flag;

	while (!PORTD_ISFR)
		;
	flag = PORTD_ISFR;
	PORTD_ISFR = ~0; //清中断标志位

	DELAY_MS(10);

	if (gpio_get(KEY_PTxn[5]) == KEY_DOWN && flag & (1 << 13)) //中键按下
	{
		onpress_M();
	}
	else if (gpio_get(KEY_PTxn[0]) == KEY_DOWN && flag & (1 << 10))
	{
		onpress_R();
	}
	else if (gpio_get(KEY_PTxn[1]) == KEY_DOWN && flag & (1 << 14))
	{
		onpress_L();
	}
	else if (gpio_get(KEY_PTxn[2]) == KEY_DOWN && flag & (1 << 11))
	{
		onpress_D();
	}
	else if (gpio_get(KEY_PTxn[3]) == KEY_DOWN && flag & (1 << 12))
	{
		onpress_U();
	}
	// else if ((gpio_get(KEY_PTxn[4]) == KEY_DOWN && flag & (1 << 7)) && p_current_state == &imgbuff_show)
	// {
	// 	flash_Picture();
	// }
	key_on = 1; //记录有按键按下
}

/*结构体的元素个数存放在colour[MAX_OPTION - 1]中 
消抖时间控制在最后几行*/
void updateUI()
{
	int i = 0;
	int m = 0;
	int n = 0;

	for (n = 0; n < 201; n++)
	{
		if (0 == strcmp(screen_data[n].data_name, "end"))
			break;
	}
	colour[MAX_OPTION - 1] = 300 * n; //记录要处理的数据个数

	m = 6 * (page - 1);
	LCD_clear(WHITE); //清屏防止上次留下残影
	for (i = 0; i < 6; i++)
	{
		if (m + i >= n)
		{
			break;
		}
		if (99 == screen_data[m + i].icrement)
		{
			LCD_str(tem_site_str[i], screen_data[m + i].data_name, BLACK, colour[m + i]); //记得回来改颜色
			LCD_str(tem_site_data[i], (*(screen_data[m + i].data_value) < 0) ? "ON" : "OFF", BLACK, WHITE);
		}
		else
		{
			LCD_str(tem_site_str[i], screen_data[m + i].data_name, BLACK, colour[m + i]); //记得回来改颜色
			LCD_numf(tem_site_data[i], (float)(*(screen_data[m + i].data_value)), BLACK, WHITE);
		}
	}
}

void UI_INIT()
{
	int i = 0;

	for (i = 0; i < MAX_OPTION; i++)
	{
		colour[i] = WHITE;
	}
	/*配置各个按键的中断*/
	port_init(PTD14, ALT1 | IRQ_FALLING | PULLUP); //下
	port_init(PTD13, ALT1 | IRQ_FALLING | PULLUP); //中
	port_init(PTD12, ALT1 | IRQ_FALLING | PULLUP); //右
	port_init(PTD11, ALT1 | IRQ_FALLING | PULLUP); //左
	port_init(PTD10, ALT1 | IRQ_FALLING | PULLUP); //上
	port_init(PTD7, ALT1 | IRQ_FALLING | PULLUP);  //flash按键
	enable_irq(PORTD_IRQn);						   //使能d对应的端口也就是按键的port
	set_vector_handler(PORTD_VECTORn, PORTD_IRQHandler);
	initFlashs();
}

/*----------各种状态下对应的5个建的操作--------*/
/*光标停留在屏幕外，此时为等待模式*/
Lcd_State wait_middle =
	{
		quit_Lcd,   //中 退出lcd,显示图像
		goto_End,   //上 去最下面
		goto_Begin, //下 去最上面
		turn_Front, //左 向前翻页
		turn_Back   //右 向后翻页
};
Lcd_State normal_page =
	{
		goto_Set,	//中
		goto_Before, //上
		goto_next,   //下
		data_Down,   //左
		data_Up		 //右
};
Lcd_State imgbuff_show =
	{
		quit_show,			 //中
		goShowDealedPicture, //上 进入显示
		go_Picture,			 //下
		takePhoto,			 //左
		doNothing			 //右
};
Lcd_State read_picture =
	{
		sendPictureToUART,			  //中	发送当前的图片
		read_picture_to_imgbuff_show, //上
		savePicture,				  //下
		readBeforePicture,			  //左
		readNextPicture				  //右
};
//显示处理过的图像
Lcd_State show_dealed_picture =
	{
		showFilterPictures,				//中
		showSobelPictures,				//上
		showDealedPictureToImgbuffShow, //下
		readBeforePicture,				//左
		readNextPicture					//右
};
