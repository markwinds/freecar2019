/*
- 用readFileToSrc和showPicture尝试是否能成功从文件读取图片
- 
*/
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#include <vector>

using namespace std;

#define src(i, j) *(src + 80 * (i) + j)
#define p(i, j) *(p + 80 * (i) + j)
#define picture(i, j) *(picture + 80 * (i) + j)

typedef struct Point
{
    int x;
    int y;
} Point;

vector<Point> ans;

char src[60][80];
char picture1[60][80];
double k1 = -0.00006;
double k2 = -0.00006;
bool isOk[60][80] = {false};
int num = 0;

void readFileToSrc()
{
    char *p = src[0];
    fstream fp;
    fp.open("road1.txt", ios::in);
    while (!fp.eof())
    {
        fp >> *(p++);
    }
    fp.close();
}

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

int main()
{
    /*******************测试读取图片****************/
    // readFileToSrc();
    // showPicture(src[0], 60, 80);

    /*******************测试设置的参数****************/
    // readFileToSrc();
    // k1 = -0.00008;
    // k2 = -0.00008;
    // setK1K2ForPicture1();
    // showPicture(picture1[0], 60, 80);

    /*******************测试最小校正表****************/
    readFileToSrc();
    for (int i = 60; i < 100; i++)
    {
        for (int j = 80; j < 200; j++)
        {
            if (updatePicture(i, j) == 4800 && updatePicture(i - 1, j) != 4800 && updatePicture(i, j - 1) != 4800)
            {
                Point temp;
                temp.x = i;
                temp.y = j;
                ans.push_back(temp);
            }
        }
    }
    for (unsigned int i = 0; i < ans.size(); i++)
    {
        cout << ans[i].x << ", " << ans[i].y << endl;
    }

    /*******************用测好的参数生成校正表****************/

    getchar();
    return 0;
}
