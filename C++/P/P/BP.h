#ifndef _BP_H
#define _BP_H

#pragma once
#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define BLNUM 60
#define BCNUM 80

#define PSIZE 4
//梯形的上底（长边）
#define PUP 270 * PSIZE
//梯形的下底（短边）
#define PDOWN 25 * PSIZE
//梯形的高
#define PHIGH 150 * PSIZE

#define src(i, j) *(src + 80 * (i) + j)
#define p(i, j) *(p + 80 * (i) + j)
#define picture(i, j) *(picture + 80 * (i) + j)
#define psize(i, j) *(psize + PUP * (i) + j)

typedef struct MyPoint
{
    int x;
    int y;
} MyPoint;

typedef struct MapPoint
{
    int x;
    int y;
    int num;
} MapPoint;

extern char src[60][80];
extern char B_picture[BLNUM][BCNUM];
extern char P_picture[PHIGH][PUP];
extern Mat B_to_P_matrix;
extern Mat P_to_B_matrix;
extern vector<MyPoint> ans;

extern double k1;
extern double k2;

extern void readFileToSrc();
extern void showPicture(char *p, int L_num, int C_num);
extern void showBPicture();
extern void showPPicture();
extern void getBPicture();
extern void getPPicture();
extern void getPToBMatrix();
extern void showImage(char *psize, int Ln, int Col);
extern void showOPPicture();
extern void showTable();
extern int updatePicture(int L_num, int C_num);
extern void setK1K2ForPicture1();
extern char picture1[60][80];

#endif