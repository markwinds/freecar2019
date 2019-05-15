#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include "common.h"
#include "include.h"

#define EDGE_CRITICAL_VALUE 15
#define src(i, j) (*((uint8 *)(src + 80 * (i) + (j))))
#define ans(i, j) (*((uint8 *)(ans + 80 * (i) + (j))))

enum FilterType
{
    LowPass,
    HighPass
};

enum FilterObject
{
    AllPoint,
    WhitePoint,
    BlackPoint
};

typedef enum FilterType FilterType;
typedef enum FilterObject FilterObject;

extern uint32 getRoot(uint32 a); //计算a的根
extern int32 getCurvature(Site_t site[]);
extern void filter(uint8 *ans, uint8 *src, FilterType filter_type, uint8 filter_size, FilterObject filter_object);
extern void sobel(uint8 *ans, uint8 *src);

#endif //__ALGORITHMS_H__
