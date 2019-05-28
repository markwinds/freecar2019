#include "include.h"

using namespace cv;
using namespace std;


PointCount point_count[60][80];
Mat B_picture = Mat::zeros(60*3, 80*3, CV_8UC1);
Mat temp_B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);
vector<Point2i> B_left_points, B_right_points, B_down_points;
int k1, k2;
int zbt1, zbt2;

/*
得到桶形失真校正后的坐标
*/
void getNewPoint(double s1, double s2)
{
	temp_B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);
	memset(point_count, 0, sizeof(point_count));
	B_left_points.clear();
	B_right_points.clear();
	B_down_points.clear();
	int L_num = 60*3;
	int C_num = 80*3;
	int half_L = L_num / 2;
	int half_C = C_num / 2;
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = (int)(x * (1 + s1 * x * x + s2 * y * y)+0.5);
			int y0 = (int)(y * (1 + s1 * x * x + s2 * y * y)+0.5);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0<60&& y0 >= 0&&y0<80)
			{
				//zbt1 = i;
				//zbt2 = j;
				uchar temp = src(x0, y0) == '0' ? 100 : 160;
				temp_B_picture.at<uchar>(i, j) = temp;
				point_count[x0][y0].num++;
				point_count[x0][y0].x += i;
				point_count[x0][y0].y += j;
			}
		}
	}
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

double getPointToLineError(double k, double b,vector<Point> point)
{
	double ans = 0;
	for (int i = 0; i < point.size(); i++)
	{
		int x = point[i].x;
		int y = point[i].y;
		ans += ((k * x - y + b) / sqrt(k*k + 1)) * ((k * x - y + b) / sqrt(k*k + 1));
	}
	return ans;
}

/*
得到直线的误差度
*/
double getLineError(double s1, double s2)
{
	getNewPoint(s1,s2);
	Vec4f line;
	double sum = 0;
	//vector<Point2i>* ps[] = { &B_left_points, &B_right_points, &B_down_points };
	//vector<Point2i>* p = ps[0];
	//for (int i = 0; i < 3; i++)
	//{
	//	zbt1 = s1;
	//	zbt2 = s2;
	//	fitLine(*p, line, DIST_L2, 0, 0.01, 0.01);
	//	double cos_theta = line[0];
	//	double sin_theta = line[1];
	//	double x0 = line[2], y0 = line[3];
	//	double k = sin_theta / cos_theta;
	//	double b = y0 - k * x0;
	//	sum+=getPointToLineError(k, b,*(p++));
	//}
	fitLine(B_left_points, line, DIST_L2, 0, 0.01, 0.01);
	double cos_theta = line[0];
	double sin_theta = line[1];
	double x0 = line[2], y0 = line[3];
	double k = sin_theta / cos_theta;
	double b = y0 - k * x0;
	sum += getPointToLineError(k, b, B_left_points);

	fitLine(B_right_points, line, DIST_L2, 0, 0.01, 0.01);
	 cos_theta = line[0];
	 sin_theta = line[1];
	 x0 = line[2], y0 = line[3];
	 k = sin_theta / cos_theta;
	 b = y0 - k * x0;
	sum += getPointToLineError(k, b, B_right_points);

	fitLine(B_down_points, line, DIST_L2, 0, 0.01, 0.01);
	 cos_theta = line[0];
	 sin_theta = line[1];
	 x0 = line[2], y0 = line[3];
	 k = sin_theta / cos_theta;
	 b = y0 - k * x0;
	sum += getPointToLineError(k, b, B_down_points);
	return sum;
}

/*
通过计算得到合适的k值
*/
void getK()
{
	double min_error = 100000; //-0.00006
	for (int s1 = 30; s1 < 80; s1++)
	{
		for (int s2 = 30; s2 < 80; s2++)
		{
			double temp = getLineError(-0.000001 * (float)s1, -0.000001 * (float)s2);
			if (temp < min_error)
			//if(s1==60&&s2==60)
			{
				min_error = temp;
				k1 = s1;
				k2 = s2;
				B_picture = temp_B_picture;
			}
		}
	}
	cout << "k1:" << k1<<endl;
	cout << "k2:" << k2 << endl;
	show_pictures_name.push_back("桶形失真还原图像");
	show_pictures.push_back(B_picture);
}