#include "common.h"
#include "include.h"

/***********************************曲率算法*****************************************************/

/*
在区间[0,a/s)找到第一个x*x>=a的数
如果不存在则返回a/2
功能：取平方根
a:要取平方根的数
*/
uint32 getRoot(uint32 a)
{
    uint32 left = 0;
    uint32 right;
    if (a > 40000) //防止mid * mid越界
        right = a / 200;
    else
        right = a;
    uint32 mid;
    while (left < right)
    {
        mid = (left + right) / 2;
        if (mid * mid >= a)
        {
            right = mid;
        }
        else
        {
            left = mid + 1;
        }
    }
    return left;
}

/*
传入3个点的坐标，计算曲率
ab叉乘ac,右手定则，向外结果为正赛道左拐，反之右拐
*/
int32 getCurvature(Site_t site[])
{
    uint32 x1 = (uint32)site[0].x;
    uint32 x2 = (uint32)site[1].x;
    uint32 x3 = (uint32)site[2].x;
    uint32 y1 = (uint32)site[0].y;
    uint32 y2 = (uint32)site[1].y;
    uint32 y3 = (uint32)site[2].y;

    uint32 x31 = x3 - x1;
    uint32 x32 = x3 - x2;
    uint32 x21 = x2 - x1;
    uint32 y31 = y3 - y1;
    uint32 y32 = y3 - y2;
    uint32 y21 = y2 - y1;
    uint32 den = getRoot((x21 * x21 + y21 * y21) * (x32 * x32 + y32 * y32) * (x31 * x31 + y31 * y31) / 10000);
    int32 num = (int32)x21 * (int32)y31 - (int32)x31 * (int32)y21;
    return num * 100 / ((int32)den);
}

/***********************************滤波算法*****************************************************/

/*
3*3滤波:
filter_type
0滤黑点
1滤白点
2滤黑白点
*/
void filter(uint8 *ans, uint8 *src, FilterType filter_type, uint8 filter_size, FilterObject filter_object)
{
    uint8 temp = filter_size >> 1;
    for (uint8 i = 0; i < temp; i++)
        for (uint8 j = 0; j < 80; j++)
        {
            ans(i, j) = src(i, j);
            ans(59 - i, j) = src(59 - i, j);
        }
    for (uint8 j = 0; j < temp; j++)
        for (uint8 i = 0; i < 60; i++)
        {
            ans(i, j) = src(i, j);
            ans(i, 79 - j) = src(i, 79 - j);
        }
    for (uint8 i = temp; i < 60 - temp; i++)
    {
        for (uint8 j = temp; j < 80 - temp; j++)
        {
            if (BlackPoint == filter_object && 0 == src(i, j) ||
                WhitePoint == filter_object && 1 == src(i, j) ||
                AllPoint == filter_object)
            {
                uint8 temp_sum = 0;
                for (uint8 m = i - temp; m < i + temp + 1; m++)
                    for (uint8 n = j - temp; n < j + temp + 1; n++)
                    {
                        temp_sum += src(m, n);
                    }
                ans(i, j) = temp_sum / (((filter_size * filter_size) >> 1) + 1);
                if (filter_type == HighPass)
                    ans(i, j) = ans(i, j) == 0 ? 1 : 0;
            }
            else
                ans(i, j) = src(i, j);
        }
    }
}

/***********************************边缘检测算法*****************************************************/
/*
sobel算子边缘检测
*/
void sobel(uint8 *ans, uint8 *src)
{
    int tempX; //x轴方向的导数
    int tempY; //y轴方向的导数
    for (uint8 j = 0; j < 80; j++)
    {
        ans(0, j) = 0;
        ans(59, j) = 0;
    }
    for (uint8 i = 0; i < 60; i++)
    {
        ans(i, 0) = 0;
        ans(i, 79) = 0;
    }
    for (uint8 i = 1; i < 59; i++)
    {
        for (uint8 j = 1; j < 79; j++)
        {
            tempX = src(i - 1, j + 1) + 2 * src(i, j + 1) + src(i + 1, j + 1) - (src(i - 1, j - 1) + 2 * src(i, j - 1) + src(i + 1, j - 1));
            tempY = src(i - 1, j - 1) + 2 * src(i - 1, j) + src(i - 1, j + 1) - (src(i + 1, j - 1) + 2 * src(i + 1, j) + src(i + 1, j + 1));
            int tempAns = tempX * tempX + tempY * tempY;
            if (tempAns > EDGE_CRITICAL_VALUE)
                ans(i, j) = 1; //超过阈值，则判定为边缘
            else
                ans(i, j) = 0;
        }
    }
}
