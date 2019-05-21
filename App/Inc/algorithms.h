#ifndef __ALGORITHMS_H__
#define __ALGORITHMS_H__

#include "common.h"
#include "include.h"

#define EDGE_CRITICAL_VALUE 15
#define src(i, j) (*((uint8 *)(src + 80 * (i) + (j))))
#define ans(i, j) (*((uint8 *)(ans + 80 * (i) + (j))))

#define TRAPEZOID_A 12
#define TRAPEZOID_B 72
#define TRAPEZOID_H 150

typedef int16 CoordinateType; //坐标点的数据类型

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

typedef struct Coordinate
{
    CoordinateType x;
    CoordinateType y;
} Coordinate;

typedef enum FilterType FilterType;
typedef enum FilterObject FilterObject;

extern int32 getRoot(int32 a); //计算a的根
extern int32 getCurvature(Coordinate site[]);
extern void filter(uint8 *ans, uint8 *src, FilterType filter_type, uint8 filter_size, FilterObject filter_object);
extern void sobel(uint8 *ans, uint8 *src);
extern Coordinate getNewCoordinate(Coordinate site);
extern Coordinate getCoordinate(CoordinateType x, CoordinateType y);
extern void getPointForCurvature(uint8 *src);

#endif //__ALGORITHMS_H__
