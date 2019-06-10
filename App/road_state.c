#include "common.h"
#include "include.h"

void doNothings()
{
}
RoadState straighta_way = {0, doNothings, doNothings};
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
}

/****************************************enter function************************************/

void outputRoadStateEnter()
{
    char name[30];
    getRoadName(name);
    printf("%s enter\n", name);
}

/****************************************exit function************************************/
void outputRoadStateExit()
{
    char name[30];
    getRoadName(name);
    printf("%s exit\n", name);
}
/*********************************************定义道路类型************************************/

// RoadState circle_left_in1 = {300, enableVerticalCircleLeftIn, unableVerticalCircleLeftIn, T0L0, clearGoStraightaFlag};
// RoadState circle_left_in2 = {0, goCircleLeft, doNothings, T0L0, doNothings};
// RoadState circle_left_out1 = {300, enableVerticalCircleLeftOut, unableVerticalCircleLeftOut, T0L0, clearVl};
// RoadState circle_left_out2 = {300, addVr, doNothings, T0L0, clearVr};
RoadState circle_left_in1 = {1000, outputRoadStateEnter, outputRoadStateExit};
RoadState circle_left_in2 = {1000, outputRoadStateEnter, outputRoadStateExit};
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
    //if (now_road_state ==)
    if (mid + hl > 1800 || mid + hr > 1800)
    {
        if (abs(vl - vr) < 10 ? hl > hr : vl > vr)
            updataRoadState(&circle_left_in1);
        else
        {
        }
    }

    // if (enable_vertical_left_out && vl > 100)
    // {
    //     updataRoadState(&circle_left_out2);
    // }
    // if (last_road_state == &circle_left_in2 && mid + hl + hr > 2300)
    // {
    //     updataRoadState(&circle_left_out1);
    // }
    // if (enable_vertical_left_in && vl > 85)
    // {
    //     updataRoadState(&circle_left_in2);
    // }
    // else if (mid + hl > 1900)
    // {
    //     updataRoadState(&circle_left_in1);
    // }
}