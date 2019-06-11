#include "common.h"
#include "include.h"

ADCMode ADC_mode = N5;
int64 time_long = 0;
int32 ADC_middle_max = 0;

void doNothings()
{
}

/******************************************定时器操作************************************/
/*
 *  @brief      关闭定时器并清楚保护位
 */
void resetTimer()
{
    pit_close(PIT0);
}

/*
 *  @brief      PIT0中断服务函数
 */
void PIT0Function()
{
    resetTimer();
    now_road_state->exit();
    now_road_state = &straighta_way;
    now_road_state->enter();
    ADC_mode = N5;
    steer_offset = 0;
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

void PIT2Function()
{
    PIT_Flag_Clear(PIT2);
    time_long++;
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
    pit_init_ms(PIT2, 1);
    set_vector_handler(PIT2_VECTORn, PIT2Function);
    enable_irq(PIT2_IRQn);
}

/*
 *  @brief      更新道路状态
 *  @param      new_road_state       新的路况
 */
void updataRoadState(RoadState *new_road_state)
{
    if (now_road_state == new_road_state)
    {
        return;
    }
    resetTimer();
    /**更新当前状态*/
    last_road_state = now_road_state;
    now_road_state->exit();
    now_road_state = new_road_state;
    /**进入状态的预备操作*/
    now_road_state->enter();
    setRoadTypeTimer(now_road_state->time);
}

void getRoadName(char *p)
{
    if (now_road_state == &straighta_way)
    {
        strcpy(p, "straighta_way");
    }
    else if (now_road_state == &circle_left_in1)
    {
        strcpy(p, "circle_left_in1");
    }
    else if (now_road_state == &circle_left_in1)
    {
        strcpy(p, "circle_left_in1");
    }
    else if (now_road_state == &circle_left_in2)
    {
        strcpy(p, "circle_left_in2");
    }
    else if (now_road_state == &circle_left_out1)
    {
        strcpy(p, "circle_left_out1");
    }
    else if (now_road_state == &circle_left_out2)
    {
        strcpy(p, "circle_left_out2");
    }
    else if (now_road_state == &circle_left_in3)
    {
        strcpy(p, "circle_left_in3");
    }
}

/****************************************enter function************************************/

void outputRoadStateEnter()
{
    char name[30];
    getRoadName(name);
    printf("%lld %s enter\n", time_long, name);
}

/****************************************exit function************************************/
void outputRoadStateExit()
{
    char name[30];
    getRoadName(name);
    printf("%lld %s exit\n", time_long, name);
}
/*********************************************定义道路类型************************************/

// RoadState circle_left_in1 = {300, enableVerticalCircleLeftIn, unableVerticalCircleLeftIn, T0L0, clearGoStraightaFlag};
// RoadState circle_left_in2 = {0, goCircleLeft, doNothings, T0L0, doNothings};
// RoadState circle_left_out1 = {300, enableVerticalCircleLeftOut, unableVerticalCircleLeftOut, T0L0, clearVl};
// RoadState circle_left_out2 = {300, addVr, doNothings, T0L0, clearVr};
RoadState straighta_way = {0, outputRoadStateEnter, outputRoadStateExit};
RoadState circle_left_in1 = {2000, outputRoadStateEnter, outputRoadStateExit};
RoadState circle_left_in2 = {7000, outputRoadStateEnter, outputRoadStateExit};
RoadState circle_left_in3 = {3000, outputRoadStateEnter, outputRoadStateExit};
RoadState circle_left_out1 = {1000, outputRoadStateEnter, outputRoadStateExit};
RoadState circle_left_out2 = {1000, outputRoadStateEnter, outputRoadStateExit};

RoadState *now_road_state = &straighta_way;
RoadState *last_road_state = &straighta_way;

/*********************************************提供给外部的判别接口************************************/
/*
 *  @brief      根据电磁的数据判断道路情况
 *  @param      mid       中间电感值
 */
void judgeRoadFromADC(int32 mid, int32 hl, int32 hr, int32 vl, int32 vr)
{
    /**状态判定*/
    if (now_road_state == &circle_left_in2 &&
        mid > 1500)
    {
        updataRoadState(&circle_left_in3);
        ADC_middle_max = 0;
    }
    else if (now_road_state == &circle_left_in1 &&
             mid + hl < 1400 &&
             hr < 1000)
    {
        updataRoadState(&circle_left_in2);
    }
    else if (now_road_state == &straighta_way &&
             mid + hl > 2200)
    {
        updataRoadState(&circle_left_in1);
        ADC_middle_max = 0;
    }
    /**状态内操作*/
    if (now_road_state == &circle_left_in1)
    {
        if (ADC_normal_vaule[0] < last_ADC_normal_vaule[0] && ADC_normal_vaule[0] < ADC_middle_max - 120)
        {
            steer_offset = -1 * (int32)param1;
        }
    }
    else if (now_road_state == &circle_left_in2)
    {
        steer_offset = -1 * (int32)param1;
    }
    else if (now_road_state == &circle_left_in3)
    {
        if (ADC_normal_vaule[0] < last_ADC_normal_vaule[0] && ADC_normal_vaule[0] < ADC_middle_max - 120)
        {
            steer_offset = param2;
        }
    }
}