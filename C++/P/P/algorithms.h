#pragma once
#include "include.h"

//using namespace cv;
using namespace std;

typedef struct PointCount
{
	int num;
	int x;
	int y;
}PointCount;

extern int k1 , k2 ;									
extern cv::Mat B_to_P;
extern int P_picture_size[2];
extern vector<cv::Point2f> point_O_B, point_B_P, point_O_P, point_B_O, point_P_B, point_P_O;


extern void getK();
extern void getTable(int s1 = k1, int s2 = k2, cv::Mat tran_matrix = B_to_P);

//extern void fuckYou();