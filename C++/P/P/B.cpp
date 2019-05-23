#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <vector>
#include "BP.h"

using namespace std;

vector<MyPoint> ans;

char src[60][80];
char B_picture[BLNUM][BCNUM];
char picture1[60][80];
//double k1 = -0.00002;
//double k2 = -0.00006;
double k1 = 0;
double k2 = 0;
bool isOk[60][80] = {false};
int num = 0;

/*
从指定的文件读取图片数据
*/
void readFileToSrc()
{
    char *p = src[0];
    fstream fp;
    fp.open("road2.txt", ios::in);
    //while (!fp.eof())
    for (int i = 0; i < 4800; i++)
    {
        fp >> *(p++);
    }
    fp.close();
}

/*
用0和1的形式显示图片
*/
void showPicture(char *p, int L_num, int C_num)
{
    for (int i = 0; i < L_num; i++)
    {
        for (int j = 0; j < C_num; j++)
        {
            cout << *(p++);
        }
        cout << endl;
    }
}

/*
设置畸变的参数
*/
void setK1K2ForPicture1()
{
    int L_num = 60;
    int C_num = 80;
    int half_L = L_num / 2;
    int half_C = C_num / 2;
    for (int i = 0; i < L_num; i++)
    {
        int x = i - half_L;
        for (int j = 0; j < C_num; j++)
        {
            int y = j - half_C;
            int x0 = round(x * (1 + k1 * x * x + k2 * y * y));
            int y0 = round(y * (1 + k1 * x * x + k2 * y * y));
            x0 += half_L;
            y0 += half_C;
            picture1[i][j] = src[x0][y0];
        }
    }
}

/*
返回B图经过桶形变换后覆盖的O图点数
*/
int updatePicture(int L_num, int C_num)
{
    num = 0;
    memset(isOk, false, sizeof(isOk));
    int half_L = L_num / 2;
    int half_C = C_num / 2;
    for (int i = 0; i < L_num; i++)
    {
        int x = i - half_L;
        for (int j = 0; j < C_num; j++)
        {
            int y = j - half_C;
            int x0 = round(x * (1 + k1 * x * x + k2 * y * y));
            int y0 = round(y * (1 + k1 * x * x + k2 * y * y));
            x0 += half_L;
            y0 += half_C;
            x0 -= half_L - 30;
            y0 -= half_C - 40;
            if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80 && !isOk[x0][y0])
            {
                isOk[x0][y0] = true;
                num++;
            }
        }
    }
    cout << num << " OK" << endl;
    return num;
}

/*
得到通过桶形变换的B图
*/
void getBPicture()
{
    int L_num = BLNUM;
    int C_num = BCNUM;
    int half_L = L_num / 2;
    int half_C = C_num / 2;
    for (int i = 0; i < L_num; i++)
    {
        int x = i - half_L;
        for (int j = 0; j < C_num; j++)
        {
            int y = j - half_C;
            int x0 = round(x * (1 + k1 * x * x + k2 * y * y));
            int y0 = round(y * (1 + k1 * x * x + k2 * y * y));
            x0 += half_L;
            y0 += half_C;
            x0 -= half_L - 30;
            y0 -= half_C - 40;
            B_picture[i][j] = (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80) ? src[x0][y0] : 0;
        }
    }
}

/*
用0和1的方式显示B图
*/
void showBPicture()
{
    getBPicture();
    showPicture(B_picture[0], BLNUM, BCNUM);
}

// int main()
// {
//     /*******************测试读取图片****************/
//     // readFileToSrc();
//     // showPicture(src[0], 60, 80);

//     /*******************测试设置的参数****************/
//     // readFileToSrc();
//     // k1 = -0.00008;
//     // k2 = -0.00008;
//     // setK1K2ForPicture1();
//     // showPicture(picture1[0], 60, 80);

//     /*******************测试最小校正表****************/
//     readFileToSrc();
//     for (int i = 60; i < 100; i++)
//     {
//         for (int j = 80; j < 200; j++)
//         {
//             if (updatePicture(i, j) == 4800 && updatePicture(i - 1, j) != 4800 && updatePicture(i, j - 1) != 4800)
//             {
//                 MyPoint temp;
//                 temp.x = i;
//                 temp.y = j;
//                 ans.push_back(temp);
//             }
//         }
//     }
//     for (unsigned int i = 0; i < ans.size(); i++)
//     {
//         cout << ans[i].x << ", " << ans[i].y << endl;
//     }

//     /*******************用测好的参数生成校正表****************/

//     getchar();
//     return 0;
// }
