#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include "common.h"
#include "include.h"

#define EDGE_CRITICAL_VALUE 15
#define src(i, j) (*((uint8 *)(src + 80 * (i) + (j))))
#define ans(i, j) (*((uint8 *)(ans + 80 * (i) + (j))))

extern uint32 getRoot(uint32 a); //计算a的根
extern uint32 getCurvature(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 x3, uint8 y3);
extern void filter(uint8 *ans, uint8 *src, uint8 filter_size);
extern void sobel(uint8 *ans, uint8 *src);

#endif //__ALGORITHMS_H__
