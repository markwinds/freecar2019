#include "common.h"
#include "include.h"

uint8 sobelAns[CAMERA_SIZE * 8]; //存储边缘检测的结果，是边缘则为1，不是边缘为0

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
uint32 getCurvature(uint8 x1, uint8 y1, uint8 x2, uint8 y2, uint8 x3, uint8 y3)
{
    uint32 x31 = (uint32)x3 - (uint32)x1;
    uint32 x32 = (uint32)x3 - (uint32)x2;
    uint32 x21 = (uint32)x2 - (uint32)x1;
    uint32 y31 = (uint32)y3 - (uint32)y1;
    uint32 y32 = (uint32)y3 - (uint32)y2;
    uint32 y21 = (uint32)y2 - (uint32)y1;
    uint32 den = getRoot((x21 * x21 + y21 * y21) * (x32 * x32 + y32 * y32) * (x31 * x31 + y31 * y31));
    uint32 num = x21 * y31 - x31 * y21;
    return num * 1000 / den;
}

/*
3*3滤波
*/
void filter(uint8 *ans, uint8 *src)
{
    for (uint8 i = 0; i < 80; i++)
    {
        ans(0, i) = src(0, i);
        ans(59, i) = src(59, i);
    }
    for (uint8 i = 0; i < 60; i++)
    {
        ans(i, 0) = src(i, 0);
        ans(i, 79) = src(i, 79);
    }
    for (uint8 i = 1; i < 59; i++)
    {
        for (uint8 j = 1; j < 79; j++)
        {
            if (i == 25 && j == 44)
            {
                printf("\n");
            }
            uint8 temp = 0;
            temp += src(i - 1, j - 1);
            temp += src(i - 1, j);
            temp += src(i - 1, j + 1);
            temp += src(i, j - 1);
            temp += src(i, j);
            temp += src(i, j + 1);
            temp += src(i + 1, j - 1);
            temp += src(i + 1, j);
            temp += src(i + 1, j + 1);
            // uint8 temp = (src(i - 1, j - 1) + src(i - 1, j) + src(i - 1, j + 1) +
            //               src(i, j - 1) + src(i, j) + src(i, j + 1) +
            //               src(i + 1, j - 1) + src(i + 1, j) + src(i + 1, j + 1));
            ans(i, j) = temp / 5;
        }
    }
}

/*
sobel算子边缘检测
*/
void sobel(uint8 *src)
{
    int tempX; //x轴方向的导数
    int tempY; //y轴方向的导数
    for (uint8 j = 0; j < 80; j++)
    {
        sobelAns(0, j) = 0;
        sobelAns(59, j) = 0;
    }
    for (uint8 i = 0; i < 60; i++)
    {
        sobelAns(i, 0) = 0;
        sobelAns(i, 79) = 0;
    }
    for (uint8 i = 1; i < 59; i++)
    {
        for (uint8 j = 1; j < 79; j++)
        {
            tempX = src(i - 1, j + 1) + 2 * src(i, j + 1) + src(i + 1, j + 1) - (src(i - 1, j - 1) + 2 * src(i, j - 1) + src(i + 1, j - 1));
            tempY = src(i - 1, j - 1) + 2 * src(i - 1, j) + src(i - 1, j + 1) - (src(i + 1, j - 1) + 2 * src(i + 1, j) + src(i + 1, j + 1));
            int ans = tempX * tempX + tempY * tempY;
            if (ans > EDGE_CRITICAL_VALUE)
                sobelAns(i, j) = 1; //超过阈值，则判定为边缘
            else
                sobelAns(i, j) = 0;
        }
    }
}
