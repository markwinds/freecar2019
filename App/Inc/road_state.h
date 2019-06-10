#ifndef _ROAD_STATE_H_
#define _ROAD_STATE_H_

#include "common.h"
#include "include.h"

typedef struct RoadState
{
    /**状态持续时间*/
    int32 time;
    /**进入该状态时要执行的操作*/
    void (*enter)();
    /**退出该状态时要执行的操作*/
    void (*exit)();
} RoadState;

extern RoadState *now_road_state;
extern RoadState *last_road_state;
extern RoadState straighta_way, circle_left_in1, circle_left_in2, circle_left_out1, circle_left_out2;

extern void judgeRoadFromADC(int32 mid, int32 hl, int32 hr, int32 vl, int32 vr);
extern void initJudgeRoad();

#endif //_ROAD_STATE_H_