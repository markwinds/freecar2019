#include "include.h"

using namespace cv;
using namespace std;

PointCount point_count[60][80];								  //小图坐标映射到大图坐标的时候用
//Mat B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);		  //用来存储桶形校正后的矩阵
//Mat temp_B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);	 //用来暂存K参数暂时最优时的B图
vector<Point2i> B_left_points, B_right_points, B_down_points; //存放经过桶形校正后的边界点
int k1=0, k2=0;													  //桶形校正的参数
int B_picture_size[] = {75,100};

//void getBPictureSize(double s1,double s2)
//{
//	const int maxr = 70;
//	const int maxc = 90;
//	bool size_enough[maxr-60][maxc-80];
//	memset(size_enough, false, sizeof(size_enough));
//	for (int m = 60; m < maxr; m++)
//	{
//		for (int n = 80; n < maxc; n++)
//		{
//			bool visit[60][80];
//			int num = 0;
//			memset(visit, false, sizeof(visit));
//			int L_num = m;
//			int C_num = n;
//			int half_L = L_num / 2;
//			int half_C = C_num / 2;
//			for (int i = 0; i < L_num; i++)
//			{
//				int x = i - half_L;
//				for (int j = 0; j < C_num; j++)
//				{
//					int y = j - half_C;
//					int x0 = ((1 + s1 * x * x + s2 * y * y) * x) > 0 ? (int)((1 + s1 * x * x + s2 * y * y) * x + 0.5) : (int)((1 + s1 * x * x + s2 * y * y) * x - 0.5);
//					int y0 = ((1 + s1 * x * x + s2 * y * y) * y) > 0 ? (int)((1 + s1 * x * x + s2 * y * y) * y + 0.5) : (int)((1 + s1 * x * x + s2 * y * y) * y - 0.5);
//					x0 += 30;
//					y0 += 40;
//					if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80 && visit[x0][y0]==false)
//					{
//						visit[x0][y0] = true;
//						num++;
//					}
//				}
//			}
//			if (num == 4800)
//			{
//				size_enough[m - 60][n - 80] = true;
//			}
//		}
//	}
//	for (int i = 1; i < maxr - 60; i++)
//	{
//		for (int j = 1; j < maxc - 80; j++)
//		{
//			if (size_enough[i][j] == true && size_enough[i - 1][j] == false && size_enough[i][j - 1] == false)
//			{
//				B_picture_size[0] = 60 + i;
//				B_picture_size[1] = 80 + j;
//				break;
//			}
//		}
//	}
//}

/**
得到桶形失真校正后的坐标
*/
void getNewPoint(double s1, double s2)
{
	//Mat temp_B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);
	//for(int i = 0; i< temp_B_picture.rows;i++)
	//{
	//	uchar* data = temp_B_picture.ptr<uchar>(i);
	//	for(int j= 0;j< temp_B_picture.cols;j++)
	//	{
	//		data[i] = 0;
	//	}
	//}
	//getBPictureSize(s1,s2);
	memset(point_count, 0, sizeof(point_count));
	B_left_points.clear();
	B_right_points.clear();
	B_down_points.clear();
	int L_num = B_picture_size[0];
	int C_num = B_picture_size[1];
	int half_L = L_num / 2;
	int half_C = C_num / 2;
	//将大图映射到小图，那么小图映射到大图的坐标就是大图映射到小图该点的所有点的坐标平均值
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = ((1 + s1 * x * x + s2 * y * y) * x) > 0 ? (int)((1 + s1 * x * x + s2 * y * y) * x + 0.5) : (int)((1 + s1 * x * x + s2 * y * y) * x - 0.5);
			int y0 = ((1 + s1 * x * x + s2 * y * y) * y) > 0 ? (int)((1 + s1 * x * x + s2 * y * y) * y + 0.5) : (int)((1 + s1 * x * x + s2 * y * y) * y - 0.5);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
				//uchar temp = src(x0, y0) == '0' ? 100 : 160;
				//temp_B_picture.at<uchar>(i, j) = temp;
				point_count[x0][y0].num++;
				point_count[x0][y0].x += i;
				point_count[x0][y0].y += j;
			}
		}
	}
	//得到边界点在B图的坐标
	for (int i = 0; i < left_points.size(); i++)
	{
		int x = left_points[i].x;
		int y = left_points[i].y;
		int x0 = point_count[x][y].x / point_count[x][y].num;
		int y0 = point_count[x][y].y / point_count[x][y].num;
		B_left_points.push_back(Point2i(x0, y0));
	}
	for (int i = 0; i < right_points.size(); i++)
	{
		int x = right_points[i].x;
		int y = right_points[i].y;
		int x0 = point_count[x][y].x / point_count[x][y].num;
		int y0 = point_count[x][y].y / point_count[x][y].num;
		B_right_points.push_back(Point2i(x0, y0));
	}
	for (int i = 0; i < down_points.size(); i++)
	{
		int x = down_points[i].x;
		int y = down_points[i].y;
		int x0 = point_count[x][y].x / point_count[x][y].num;
		int y0 = point_count[x][y].y / point_count[x][y].num;
		B_down_points.push_back(Point2i(x0, y0));
	}
}




void getBPicture()
{
	//getBPictureSize(-0.000001 * k1, -0.000001 * k2);
	int L_num = B_picture_size[0];
	int C_num = B_picture_size[1];
	Mat B_picture = Mat::zeros(L_num, C_num, CV_8UC1);
	int half_L = L_num / 2;
	int half_C = C_num / 2;
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = (int)(x * (1 + -0.000001 * k1 * x * x + -0.000001 * k2 * y * y) + 0.5);
			int y0 = (int)(y * (1 + -0.000001 * k1 * x * x + -0.000001 * k2 * y * y) + 0.5);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
				uchar temp = src(x0, y0) == '0' ? 100 : 160;
				B_picture.at<uchar>(i, j) = temp;
			}
		}
	}
	imshow("jjj", B_picture);
}

/**
通过计算得到合适的k值
*/
void getK()
{
	//double min_error = 1000000; //-0.00006
	//for (int s1 = 10; s1 < 80; s1++)
	//{
	//	for (int s2 = 10; s2 < 80; s2++)
	//	{
	//		if(s1==35&&s2==0)
	//		{
	//			double temp = getLineError(-0.000001 * (float)s1, -0.000001 * (float)s2);
	//			cout << "s1:" << s1 << " s2:" << s2 << " error:" << temp << endl;
	//	/*		if (temp < min_error)

	//			{*/
	//				min_error = temp;
	//				k1 = s1;
	//				k2 = s2;
	//				B_picture = temp_B_picture;
	//			/*}*/
	//		}
	//		
	//	}
	//}
	//k1 = 90;
	//k2 = 0;
	//getBPicture();
	//cout << "最优k1:" << k1 << endl;
	//cout << "最优k2:" << k2 << endl;
	//imshow("jjj", B_picture);
	////show_pictures_name.push_back("桶形失真还原图像");
	////show_pictures.push_back(B_picture);

	int s1 = 0, s2 = 0;
	while (true)
	{
		getNewPoint(-0.000001 * s1, -0.000001 * s2);
		int size = (int)B_down_points.size();
		int x1 = B_down_points[0].x;
		int x2= B_down_points[size/2].x;
		int x3= B_down_points[size-1].x;
		int x4 = B_down_points[size/4].x;
		int x5 = B_down_points[size*3/4].x;
		cout << x1 << " " << x2 << " " << x3 << " " << endl;
		if ((x1 - x2) * (x2 - x3) >= 0)
		{
			break;
		}
		s2++;
	}
	//while (true)
	//{
	//	getNewPoint(-0.000001 * s1, -0.000001 * s2);
	//	int size = B_right_points.size();
	//	int x1 = B_right_points[0].x;
	//	int x2 = B_right_points[size / 2].x;
	//	int x3 = B_right_points[size - 1].x;
	//	int y1 = B_right_points[0].y;
	//	int y2 = B_right_points[size / 2].y;
	//	int y3 = B_right_points[size - 1].y;
	//	if ((x1 - x2)/(y1-y2) >= (x2 - x3)/(y2-y3))
	//	{
	//		break;
	//	}
	//	s1++;
	//}
	k1 = s1;
	k2 = s2;
	getBPicture();
	cout << "最优k1:" << k1 << endl;
	cout << "最优k2:" << k2 << endl;
	
}


void getPPicture()
{
	vector<Point2f> B_points(4);
	B_points[0] = B_left_points[0];
	B_points[1] = B_left_points[B_left_points.size()-1];
	B_points[2] = B_right_points[B_right_points.size() - 1];
	B_points[3] = B_right_points[0];
	vector<Point2f> P_points(4);
	P_points[0] = Point2f(0, 0);
	P_points[1] = Point2f(10, 0);
	P_points[2] = Point2f(10, 10);
	P_points[3] = Point2f(0, 10);

	Mat B_to_P = getPerspectiveTransform(B_points, P_points);
	vector<Point2f> point, point_site_in_new_picture;
	for (int i = B_left_points[0].x-20; i < B_left_points[B_left_points.size() - 1].x+20; i++)
	{
		for (int j = B_left_points[B_left_points.size() - 1].y - 20; j < B_right_points[B_left_points.size() - 1].y + 20; j++)
		{
			point.push_back(Point2f((float)i, (float)j));
		}
	}
	perspectiveTransform(point, point_site_in_new_picture, B_to_P);
	int row_max = -1;
	int row_min = 20000;
	int col_max = -1;
	int col_min = 20000;
	for (int i = 0; i < point_site_in_new_picture.size(); i++)
	{
		int x = (int)(point_site_in_new_picture[i].x + 0.5);
		int y = (int)(point_site_in_new_picture[i].y + 0.5);
		if (x > row_max)
			row_max = x;
		if (x < row_min)
			row_min = x;
		if (y > col_max)
			col_max = y;
		if (y < col_min)
			col_min = y;
	}
	for (int i = 0; i < point_site_in_new_picture.size(); i++)
	{
		point_site_in_new_picture[i].x-=row_min;
		point_site_in_new_picture[i].y -=col_min;
	}
	Mat P_picture= Mat::zeros(row_max-row_min+1, col_max - col_min + 1, CV_8UC1);
	int h = 0;
	for (int i = B_left_points[0].x - 20; i < B_left_points[B_left_points.size() - 1].x + 20; i++)
	{
		for (int j = B_left_points[B_left_points.size() - 1].y - 20; j < B_right_points[B_left_points.size() - 1].y + 20; j++)
		{
			int x = (int)(point_site_in_new_picture[h].x + 0.5);
			int y = (int)(point_site_in_new_picture[h++].y + 0.5);
			//P_picture.at<uchar>(x, y) = B_picture.at<uchar>(i, j);
		}
	}
	show_pictures_name.push_back("校正结果");
	show_pictures.push_back(P_picture);
}