#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "BP.h"

using namespace cv;
using namespace std;

char P_picture[PHIGH][PUP];
vector<Point2f> B_points(4);
vector<Point2f> P_points(4);
Mat B_to_P_matrix;
Mat P_to_B_matrix;

/*
初始化透视变换的8个映射点
*/
void initPoints()
{
	B_points[0] = Point2f(0, 0);
	B_points[1] = Point2f(BLNUM, 0);
	B_points[2] = Point2f(BLNUM, BCNUM);
	B_points[3] = Point2f(0, BCNUM);
	P_points[0] = Point2f(0, 0);
	P_points[1] = Point2f(PHIGH, (PUP-PDOWN)/2);
	P_points[2] = Point2f(PHIGH, PUP- (PUP - PDOWN) / 2);
	P_points[3] = Point2f(0, PUP);
}

/*
得到B图到P图的转化矩阵
*/
void getBToPMatrix()
{
	initPoints();
	B_to_P_matrix = getPerspectiveTransform(B_points, P_points);
}

/*
得到P图到B图的转化矩阵
*/
void getPToBMatrix()
{
	initPoints();
	P_to_B_matrix = getPerspectiveTransform(P_points,B_points);
}

/*
通过getPToBMatrix得到P图每个坐标点变换到B图的对应坐标，得到P图
*/
void getPPicture()
{
	getPToBMatrix();
	vector<Point2f> point,point_site_in_new_picture;
	for (int i = 0; i < PHIGH; i++)
	{
		for (int j = 0; j < PUP; j++)
		{
			point.push_back(Point2f(i, j));
		}
	}
	//一个大小和P图相同的point_site_in_new_picture向量存放每个P图点对应于B图的坐标
	//P图坐标 对应的B图坐标 P到B的转化矩阵
	perspectiveTransform(point, point_site_in_new_picture, P_to_B_matrix);
	int h = 0;
	for (int i = 0; i < PHIGH; i++)
	{
		for (int j = 0; j < PUP; j++)
		{
			int x = point_site_in_new_picture[h].x;
			int y = point_site_in_new_picture[h++].y;
			P_picture[i][j] = (x >= 0 && x < BLNUM && y >= 0 && y < BCNUM) ? B_picture[x][y] : 0;
		}
	}
}

/*
以图片的形式显示点阵
*/
void showImage(char* psize,int Ln,int Col)
{
	Mat img = Mat::zeros(Ln, Col, CV_8UC1);
	for (int i = 0; i < Ln; i++)
	{
		uchar* p = img.ptr<uchar>(i);
		for (int j = 0; j < Col; j++)
		{
			p[j] = psize(i,j) == 0 ? 0 : (psize(i, j) == '0' ? 50 : 200);
		}
	}
	imshow("Image", img);
	waitKey();
}

/*
以图片的形式显示P图
*/
void showPPicture()
{
	getPPicture();
	showImage(P_picture[0], PHIGH, PUP);
}



