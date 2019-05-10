#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include "common.h"
#include "include.h"

extern int getRoot(int a); //计算a的根
extern uint32 getCurvature(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 x3, uint8 y3);
extern void filter(uint8 *ans, uint8 *src);

#endif //__ALGORITHMS_H__
