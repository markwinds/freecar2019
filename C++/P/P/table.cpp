#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "BP.h"

using namespace cv;
using namespace std;

MapPoint O_B_table[60][80];
MapPoint B_P_table[BLNUM][BCNUM];
MapPoint O_P_table[60][80];

void getOBTable()
{
	memset(O_B_table, 0, sizeof(O_B_table));
	double k1 = -0.00006;
	double k2 = -0.00006;
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
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
				O_B_table[x0][y0].num++;
				O_B_table[x0][y0].x += i;
				O_B_table[x0][y0].y += j;
			}
		}
	}
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			O_B_table[i][j].x /= O_B_table[i][j].num;
			O_B_table[i][j].y /= O_B_table[i][j].num;
		}
	}
}

void getBPTable()
{
	getPToBMatrix();
	vector<Point2f> point, point_site_in_new_picture;
	for (int i = 0; i < PHIGH; i++)
	{
		for (int j = 0; j < PUP; j++)
		{
			point.push_back(Point2f(i, j));
		}
	}
	perspectiveTransform(point, point_site_in_new_picture, P_to_B_matrix);
	int h = 0;
	for (int i = 0; i < PHIGH; i++)
	{
		for (int j = 0; j < PUP; j++)
		{
			int x = point_site_in_new_picture[h].x;
			int y = point_site_in_new_picture[h++].y;
			if (x >= 0 && x < BLNUM && y >= 0 && y < BCNUM)
			{
				B_P_table[x][y].num++;
				B_P_table[x][y].x += i;
				B_P_table[x][y].y += j;
			}
		}
	}
	for (int i = 0; i < BLNUM; i++)
	{
		for (int j = 0; j < BCNUM; j++)
		{
			B_P_table[i][j].x /= B_P_table[i][j].num;
			B_P_table[i][j].y /= B_P_table[i][j].num;
		}
	}
}

void getOPTable()
{
	getOBTable();
	getBPTable();
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			MapPoint temp;
			temp = O_B_table[i][j]; //temp代表O图(i,j)对应的B图的点的坐标
			int x = temp.x;
			int y = temp.y;
			temp = B_P_table[x][y]; //temp代表O图(i,j)对应的P图的点的坐标
			O_P_table[i][j] = temp;
		}
	}
}

void showOPPicture()
{
	readFileToSrc();
	getOPTable();
	memset(P_picture, 0, sizeof(P_picture));
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			int x = O_P_table[i][j].x;
			int y = O_P_table[i][j].y;
			P_picture[x][y] = src[i][j];
		}
	}
	showImage(P_picture[0], PHIGH, PUP);
}

void showTable()
{
	getOPTable();
	int num = 4800;
	MapPoint *p = O_P_table[0];
	cout << "OP_table[][2]=" << endl;
	cout << "{" << endl;
	while (num--)
	{
		int x = p->x;
		int y = p->y;
		cout << "{" << x << "," << y << "}";
		if (num != 0)
		{
			cout << ", ";
		}
		p++;
		if (num % 10 == 0)
		{
			cout << endl;
		}
	}
	cout << "}" << endl;
}