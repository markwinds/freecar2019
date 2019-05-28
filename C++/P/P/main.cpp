#include "include.h"
using namespace cv;
using namespace std;
int main()
{
	readFileToSrc();
	getBorder();
	//for (int i = 0; i < left_points.size(); i++)
	//{
	//	int x = left_points[i].x;
	//	int y = left_points[i].y;
	//	src(x, y) = 255;
	//}
	//for (int i = 0; i < right_points.size(); i++)
	//{
	//	int x = right_points[i].x;
	//	int y = right_points[i].y;
	//	src(x, y) = 255;
	//}
	//for (int i = 0; i < down_points.size(); i++)
	//{
	//	int x = down_points[i].x;
	//	int y = down_points[i].y;
	//	src(x, y) = 0;
	//}
	//for (int i = 0; i < 60; i++)
	//{
	//	//uchar* p = src.ptr<uchar>(i);
	//	for (int j = 0; j < 80; j++)
	//	{
	//		src(i, j) = src(i, j) == '0' ? 100 : (src(i, j) == '1' ? 160: src(i, j));
	//	}
	//}
	//showBigPicture(src, 600, 800);
	getK();
	showPicture();
	
	return 0;
}