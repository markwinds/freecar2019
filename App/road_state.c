#include "common.h"
#include "include.h"

/**状态保护位*/
uint8 protect_flag = 0;
uint8 go_straighta_flag = 0;

uint8 enable_vertical_left_in = 0;
uint8 enable_vertical_left_out = 0;

uint8 add_vl = 0;
uint8 add_vr = 0;

int PIT2_count_num = 0;

/******************************************定时器操作************************************/
/*
 *  @brief      关闭定时器并清楚保护位
 */
void resetTimer()
{
    pit_close(PIT0);
    protect_flag = 0;
}

/*
 *  @brief      PIT0中断服务函数
 */
void PIT0Function()
{
    resetTimer();
    /**执行状态对应的中断函数*/
    now_road_state->deal_timer();
}

void PIT2Function()
{
    PIT_Flag_Clear(PIT2);
    /**如果保护位关闭，则判定是否要清空路况*/
    if (protect_flag == 0)
    {
        PIT2_count_num++;
        if (PIT2_count_num > 10) //1s
        {
            now_road_state->out_state();
            last_road_state = now_road_state == &straighta_way ? last_road_state : now_road_state;
            now_road_state = &straighta_way;
            led_turn(LED3);
            PIT2_count_num = 0;
        }
    }
    else
    {
        PIT2_count_num = 0;
    }
}

/*
 *  @brief      开启PIT0定时器和中断
 *  @param      time_long       定时的时长
 */
void setRoadTypeTimer(int16 time_long)
{
    if (time_long <= 0)
    {
        return;
    }
    pit_init_ms(PIT0, time_long);
}

/***************************************相关函数************************************/
/*
 *  @brief      初始化道路判断的定时器中断
 */
void initJudgeRoad()
{
    pit_close(PIT0);
    enable_irq(PIT0_IRQn);
    set_vector_handler(PIT0_VECTORn, PIT0Function);
    /**设置PIT2定时器*/
    pit_init_ms(PIT2, 100);
    set_vector_handler(PIT2_VECTORn, PIT2Function);
    enable_irq(PIT2_IRQn);
}

/*
 *  @brief      更新道路状态
 *  @param      new_road_state       新的路况
 */
void updataRoadState(RoadState *new_road_state)
{
    /**更新当前状态*/
    last_road_state = now_road_state;
    now_road_state->out_state();
    now_road_state = new_road_state;
    /**开启状态保护*/
    protect_flag = new_road_state->protect_time <= 0 ? 0 : 1;
    /**进入状态的预备操作*/
    now_road_state->enter_state();
    setRoadTypeTimer(now_road_state->protect_time);
}

void addVertical(int32 *left, int32 *right)
{
    if (add_vl)
    {
        *left += param2;
    }
    if (add_vr)
    {
        *right += param2;
    }
}

/*********************************************doNothing************************************/
void doNothings()
{
}

/****************************************enter function************************************/
/*
 *  @brief      用蜂鸣器发出该路况对应的声音
 */
void makeSound()
{
    tellMeRoadType(now_road_state->sound_type);
}

void enableVerticalCircleLeftIn()
{
    enable_vertical_left_in = 1;
    go_straighta_flag = 1;
    openBuzzer();
}

void goCircleLeft()
{
    setSteer(-50);
    DELAY_MS(param1);
    //closeBuzzer();
}

void enableVerticalCircleLeftOut()
{
    enable_vertical_left_out = 1;
    //add_vl = 1;
    setSteer(-45);
    DELAY_MS(200);
    openBuzzer();
}

void addVr()
{
    //add_vr = 1;
    setSteer(50);
    DELAY_MS(100);
}

/****************************************out function************************************/
void unableVerticalCircleLeftIn()
{
    enable_vertical_left_in = 0;
    closeBuzzer();
}

void unableVerticalCircleLeftOut()
{
    enable_vertical_left_out = 0;
    closeBuzzer();
}

/****************************************interrupt function************************************/

// void checkSmallCircleLeft()
// {
//     resetTimer();
//     int32 mid = getADCVaule(horizontal_middle);
//     int32 left = getADCVaule(horizontal_left);
//     if (mid + left > 1800)
//     {
//         setSteer(-50);
//         DELAY_MS(2000);
//     }
// }

void clearGoStraightaFlag()
{
    go_straighta_flag = 0;
}

void clearVl()
{
    add_vl = 0;
}

void clearVr()
{
    add_vr = 0;
}

/*********************************************定义道路类型************************************/
RoadState straighta_way = {0, doNothings, doNothings, T2L1L1, resetTimer};

RoadState circle_left_in1 = {300, enableVerticalCircleLeftIn, unableVerticalCircleLeftIn, T0L0, clearGoStraightaFlag};
RoadState circle_left_in2 = {0, goCircleLeft, doNothings, T0L0, doNothings};
RoadState circle_left_out1 = {300, enableVerticalCircleLeftOut, unableVerticalCircleLeftOut, T0L0, clearVl};
RoadState circle_left_out2 = {300, addVr, doNothings, T0L0, clearVr};

RoadState *now_road_state = &straighta_way;
RoadState *last_road_state = &straighta_way;

/*********************************************提供给外部的判别接口************************************/
/*
 *  @brief      根据电磁的数据判断道路情况
 *  @param      mid       中间电感值
 */
void judgeRoadFromADC(int32 mid, int32 hl, int32 hr, int32 vl, int32 vr)
{
    if (protect_flag)
    {
        return;
    }
    if (enable_vertical_left_out && vl > 100)
    {
        updataRoadState(&circle_left_out2);
    }
    if (last_road_state == &circle_left_in2 && mid + hl + hr > 2300)
    {
        updataRoadState(&circle_left_out1);
    }
    if (enable_vertical_left_in && vl > 85)
    {
        updataRoadState(&circle_left_in2);
    }
    else if (mid + hl > 1900)
    {
        updataRoadState(&circle_left_in1);
    }
}