#include "common.h"
#include "include.h"

Coordinate points_coordinate[250];
uint8 father[250]; //最后一个下标249
uint8 point_num = 0;
Coordinate point6[6];
uint8 useful_line[] = {10, 52};

/***********************************循环查找*****************************************************/
uint8 isNotZero(uint8 a) //判断条件成立的函数，当条件为真时，循环查找结束.当传入的值不为0时为真
{
    return a == 0 ? 0 : 1;
}
/*
*/
uint8 checkValueInSrc(uint8 *src, CheckType Ln_Col, int8 constant, uint8 begin, uint8 end, uint8 (*judge)(uint8 a))
{
    if (begin < end)
    {
        if (Ln_Col == Ln) //行不变
        {
            int j;
            for (j = begin; j < end; j++)
            {
                if (judge(src(constant, j)) == 1)
                {
                    break;
                }
            }
            return j;
        }
        else
        {
            int i;
            for (i = begin; i < end; i++)
            {
                if (judge(src(i, constant)) == 1)
                {
                    break;
                }
            }
            return i;
        }
    }
    else
    {
        if (Ln_Col == Ln)
        {
            int j;
            for (j = begin; j > end; j--)
            {
                if (judge(src(constant, j)) == 1)
                {
                    break;
                }
            }
            return j;
        }
        else
        {
            int i;
            for (i = begin; i > end; i--)
            {
                if (judge(src(i, constant)) == 1)
                {
                    break;
                }
            }
            return i;
        }
    }
}

/***********************************坐标初始化*****************************************************/
Coordinate getCoordinate(CoordinateType x, CoordinateType y)
{
    Coordinate temp = {x, y};
    return temp;
}

/***********************************曲率算法*****************************************************/

/*
在区间[0,a/s)找到第一个x*x>=a的数
如果不存在则返回a/2
功能：取平方根
a:要取平方根的数
*/
int32 getRoot(int32 a)
{
    int32 left = 0;
    int32 right;
    if (a > 40000) //防止mid * mid越界
        right = a / 200;
    else
        right = a;
    int32 mid;
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
int32 getCurvature(Coordinate site[])
{
    int32 x1 = (int32)site[0].x;
    int32 x2 = (int32)site[1].x;
    int32 x3 = (int32)site[2].x;
    int32 y1 = (int32)site[0].y;
    int32 y2 = (int32)site[1].y;
    int32 y3 = (int32)site[2].y;

    int32 x31 = x3 - x1;
    int32 x32 = x3 - x2;
    int32 x21 = x2 - x1;
    int32 y31 = y3 - y1;
    int32 y32 = y3 - y2;
    int32 y21 = y2 - y1;
    int32 den = getRoot((x21 * x21 + y21 * y21) * (x32 * x32 + y32 * y32) * (x31 * x31 + y31 * y31) / 10000);
    int32 num = x21 * y31 - x31 * y21;
    return num * 100 / den;
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

/***********************************梯形校正算法*****************************************************/
Coordinate getNewCoordinate(Coordinate site)
{
    CoordinateType x = site.x;
    CoordinateType y = site.y;
    Coordinate ans;
    ans.x = x * TRAPEZOID_H / 60;
    ans.y = (y - 40) * ((TRAPEZOID_B - TRAPEZOID_A) * (60 - x) / 60 + TRAPEZOID_A) / 40;
    return ans;
}

/***********************************并查集判连续性*****************************************************/
void initCheckAndSet()
{
    for (uint8 i = 0; i < 250; i++)
    {
        father[i] = i;
    }
    point_num = 1; //边缘点的id从1开始
}

void setPointId(uint8 *src)
{
    for (uint8 i = useful_line[0]; i < useful_line[1]; i++) //这里改扫描的范围
    {
        for (int j = 1; j < 79; j++) //这里改扫描的范围
        {
            if (1 == src(i, j) && point_num < 250) //这里不能写<256，因为当循环结束时point_id的值是256的话会有很多地方内存超限
            {
                src(i, j) = point_num;
                points_coordinate[point_num++] = getCoordinate((CoordinateType)i, (CoordinateType)j);
            }
        }
    }
}

uint8 findFather(uint8 x)
{
    uint8 a = x, z;
    while (x != father[x])
    {
        x = father[x];
    }
    while (a != father[a])
    {
        z = a;
        a = father[a];
        father[z] = x;
    }
    return x;
}

void Union(uint8 a, uint8 b)
{
    uint8 fa, fb;
    fa = findFather(a);
    fb = findFather(b);
    if (fa != fb)
    {
        if (fa < fb)
            father[fb] = fa;
        else
            father[fa] = fb;
    }
}

/*
id:输入边缘点的id编号
*/
void union9Point(uint8 id, uint8 *src)
{
    Coordinate point = points_coordinate[id];
    for (int i = point.x - 1; i < point.x + 2; i++)
    {
        for (int j = point.y - 1; j < point.y + 2; j++)
        {
            if (src(i, j) != 0 && i >= useful_line[0] && i < useful_line[1] && j > 0 && j < 79) //这里改扫描的范围
                Union(src(i, j), id);
        }
    }
}

void unionPoints(uint8 *src)
{
    for (int i = 1; i < point_num; i++) //id从1开始
    {
        union9Point(i, src);
    }
}

/*
画出所给点所在连续边的曲率3点
*/
void get3PointCoordinate(uint8 point_id, int a)
{
    int num = 0;
    int id[256];
    for (int i = 1; i < point_num; i++)
    {
        //if (findFather(i) == findFather(point_id))
        if (father[i] == father[point_id])
        {
            id[num++] = i;
        }
    }
    point6[a + 0] = points_coordinate[id[num >> 2]];
    point6[a + 1] = points_coordinate[id[num >> 1]];
    point6[a + 2] = points_coordinate[id[(num >> 2) * 3]];
}

/*
得到计算曲率的6点，并存放在point6数组中
*/
int getPointForCurvature(uint8 *src)
{
    uint8 check_col[] = {1, 78}; //查找两边边缘线的时候查找列
    initCheckAndSet();
    setPointId(src);
    if (point_num >= 251)
    {
        return -1; //点数过多返回-1
    }
    int ans = 0;
    unionPoints(src);
    for (uint8 h = 0; h < 2; h++)
    {
        uint8 temp;
        temp = checkValueInSrc(src, Col, check_col[h], useful_line[1] - 1, useful_line[0] + 10, isNotZero); //左边列查找边缘
        if (temp != useful_line[0] + 10)
        {
            get3PointCoordinate(src(temp, check_col[h]), h * 3);
            ans += 1 + h;
        }
        else
        {
            temp = checkValueInSrc(src, Ln, useful_line[1] - 1, h == 0 ? 1 : 78, h == 0 ? 78 : 1, isNotZero); //左边行查找边缘
            if (temp != h == 0 ? 78 : 1)
            {
                get3PointCoordinate(src(useful_line[1] - 1, temp), h * 3);
                ans += 1 + h;
            }
        }
    }

    // for (uint8 h = 0; h < 2; h++)//h等于0时搜左3点，h为1时搜右3点
    // {
    //     uint8 flag = 0;
    //     for (int i = useful_line[1] - 1; i >= useful_line[0] + 10; i--) //这里改扫描的范围
    //     {
    //         if (src(i, check_line[h]) != 0)
    //         {
    //             ans += h + 1;
    //             flag = 1;
    //             get3PointCoordinate(src(i, check_line[h]), h * 3);
    //             break;
    //         }
    //     }
    //     if (!flag)
    //     {
    //         if (h == 0)
    //         {
    //             for (uint8 j = 1; j < 79; j++)
    //             {
    //                 if (src(useful_line[1], j) != 0)
    //                 {
    //                     ans += h + 1;
    //                     get3PointCoordinate(src(useful_line[1], j), h * 3);
    //                     break;
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             for (uint8 j = 79; j > 0; j--)
    //             {
    //                 if (src(useful_line[1], j) != 0)
    //                 {
    //                     ans += h + 1;
    //                     get3PointCoordinate(src(useful_line[1], j), h * 3);
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }
    for (int i = 0; i < 6; i++)
    {
        LCDShowBigPoint(point6[i], BLUE);
        point6[i] = getNewCoordinate(point6[i]);
    }
    return ans;
}

void showCurvature(uint8 *src)
{
    int success_flag = 0;
    int32 left_curvature = -1;
    int32 right_curvature = -1;
    success_flag = getPointForCurvature(src);
    switch (success_flag)
    {
    case 3: //两边都有曲率
        left_curvature = getCurvature(point6);
        right_curvature = getCurvature(point6 + 3);
        break;
    case 1: //左边有曲率
        left_curvature = getCurvature(point6);
        break;
    case 2:
        right_curvature = getCurvature(point6 + 3);
        break;
    default:
        break;
    }
    LCDShowNumDefule(80, 20, left_curvature);
    LCDShowNumDefule(80, 60, right_curvature);
}