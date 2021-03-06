#pragma once
#include "include.h"

//using namespace cv;
using namespace std;

#define src(i, j) src.at<uchar>((i), (j))
#define img(i, j) img.at<uchar>((i), (j))
#define ans(i, j) ans.at<uchar>((i), (j))

extern cv::Mat src;
extern vector<cv::Point2i> left_points, right_points, down_points;
extern vector<cv::Mat> show_pictures;
extern vector<string> show_pictures_name;

extern void readFileToSrc();
extern void getBorder();
extern void showBigPicture(cv::Mat img, int Ln, int Col);
extern void showPicture();
extern void savePicture();
extern void getPPicture();
extern cv::Mat getBigPicture(cv::Mat img, int Ln, int Col);