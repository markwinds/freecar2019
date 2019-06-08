#ifndef _ROAD_STATE_H_
#define _ROAD_STATE_H_

#include "common.h"
#include "include.h"

typedef struct RoadState
{
    /**状态保护时间*/
    int32 protect_time;
    /**进入该状态时要执行的操作*/
    void (*enter_state)();
    /**退出该状态时要执行的操作*/
    void (*out_state)();
    /**蜂鸣器的响声类型*/
    SoundType sound_type;
    /**中断的处理函数，在这里可以加保护位清掉以后的处理*/
    void (*deal_timer)();
} RoadState;

extern RoadState *now_road_state;
extern RoadState *last_road_state;
extern uint8 protect_flag;
extern RoadState straighta_way, big_circle_left, big_circle_right, small_circle_left, small_circle_right;

extern void judgeRoadFromADC(int32 mid, int32 left, int32 right);
extern void checkSmallCircleLeft();
#endif //_ROAD_STATE_H_