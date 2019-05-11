#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include "common.h"
#include "include.h"

#define EDGE_CRITICAL_VALUE 10

extern uint8 sobelAns[CAMERA_SIZE * 8]; //存储边缘检测的结果，是边缘则为1，不是边缘为0

extern int getRoot(int a); //计算a的根
extern uint32 getCurvature(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 x3, uint8 y3);
extern void filter(uint8 *ans, uint8 *src);

#endif //__ALGORITHMS_H__
