#pragma once
#include "include.h"

using namespace cv;
using namespace std;

typedef struct PointCount
{
	int num;
	int x;
	int y;
}PointCount;

extern int k1 , k2 ;									
extern Mat B_to_P;


extern void getK();
extern void getTable(int s1 = k1, int s2 = k2, Mat tran_matrix = B_to_P);
extern void fuckYou();