#include "common.h"
#include "include.h"

/**状态保护位*/
uint8 protect_flag = 0;


/******************************************定时器操作************************************/
/*
 *  @brief      开启PIT0定时器和中断
 *  @param      time_long       定时的时长
 *  @param      deal_function   中断处理函数
 */
void setRoadTypeTimer(int16 time_long, void (*deal_function)())
{
    if (time_long <= 0)
    {
        return;
    }
    set_vector_handler(PIT0_VECTORn, deal_function);
    pit_init_ms(PIT0, time_long);
}

void resetTimer()
{
    pit_close(PIT0);
    protect_flag = 0;
}

/***************************************相关函数************************************/
/*
 *  @brief      初始化道路判断的定时器中断
 */
void initJudgeRoad()
{
    pit_close(PIT0);
    enable_irq(PIT0_IRQn);
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
    setRoadTypeTimer(now_road_state->protect_time, now_road_state->deal_timer);
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

/****************************************interrupt function************************************/

void checkSmallCircleLeft()
{
    resetTimer();
    int32 mid = getADCVaule(horizontal_middle);
    int32 left = getADCVaule(horizontal_left);
    if (mid + left > 1800)
    {
        setSteer(-50);
        DELAY_MS(200);
    }
}

/*********************************************定义道路类型************************************/
RoadState straighta_way = {0, doNothings, doNothings, T2L1L1, resetTimer};
// RoadState big_circle_left = {0, doNothings, doNothings, T2L1L1, resetTimer};
// RoadState big_circle_right = {0, doNothings, doNothings, T2L1L1, resetTimer};
RoadState small_circle_left_in = {500, makeSound, doNothings, T1L5, checkSmallCircleLeft};
//RoadState small_circle_left1 = {3000, makeSound, doNothings, T1L5, checkSmallCircleLeft};
// RoadState small_circle_right = {0, doNothings, doNothings, T1L5, resetTimer};

RoadState *now_road_state = &straighta_way;
RoadState *last_road_state = &straighta_way;

/*********************************************提供给外部的判别接口************************************/
void judgeRoadFromADC(int32 mid, int32 left, int32 right)
{
    if (protect_flag)
    {
        return;
    }
    if (mid + left > 1900)
    {
        updataRoadState(&small_circle_left_in);
    }
    else if (mid + right > 1900)
    {
        //updataRoadState(&small_circle_right);
    }
}