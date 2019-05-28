#include "include.h"

using namespace cv;
using namespace std;

PointCount point_count[60][80];								  //Сͼ����ӳ�䵽��ͼ�����ʱ����
Mat B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);		  //�����洢Ͱ��У����ľ���
Mat temp_B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);	 //�����ݴ�K������ʱ����ʱ��Bͼ
vector<Point2i> B_left_points, B_right_points, B_down_points; //��ž���Ͱ��У����ı߽��
int k1=0, k2=0;													  //Ͱ��У���Ĳ���

/**
�õ�Ͱ��ʧ��У���������
*/
void getNewPoint(double s1, double s2)
{
	//temp_B_picture = Mat::zeros(60 * 3, 80 * 3, CV_8UC1);
	for(int i = 0; i< temp_B_picture.rows;i++)
	{
		uchar* data = temp_B_picture.ptr<uchar>(i);
		for(int j= 0;j< temp_B_picture.cols;j++)
		{
			data[i] = 0;
		}
	}

	memset(point_count, 0, sizeof(point_count));
	B_left_points.clear();
	B_right_points.clear();
	B_down_points.clear();
	int L_num = 60 * 3;
	int C_num = 80 * 3;
	int half_L = L_num / 2;
	int half_C = C_num / 2;
	//����ͼӳ�䵽Сͼ����ôСͼӳ�䵽��ͼ��������Ǵ�ͼӳ�䵽Сͼ�õ�����е������ƽ��ֵ
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = (int)(x * (1 + s1 * x * x + s2 * y * y) + 0.5);
			int y0 = (int)(y * (1 + s1 * x * x + s2 * y * y) + 0.5);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
				uchar temp = src(x0, y0) == '0' ? 100 : 160;
				temp_B_picture.at<uchar>(i, j) = temp;
				point_count[x0][y0].num++;
				point_count[x0][y0].x += i;
				point_count[x0][y0].y += j;
			}
		}
	}
	//�õ��߽����Bͼ������
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

/**
������֪�ĵ㼯��ָ��ֱ�ߵ���С�������
*/
double getPointToLineError(double k, double b, vector<Point> point)
{
	double ans = 0;
	for (int i = 0; i < point.size(); i++)
	{
		int x = point[i].x;
		int y = point[i].y;
		ans += ((k * x - y + b) / sqrt(k * k + 1)) * ((k * x - y + b) / sqrt(k * k + 1));
	}
	return ans;
}

/**
�õ�һ���㼯��ֱ�ߵ�����
*/
double getLineError(double s1, double s2)
{
	getNewPoint(s1, s2);
	Vec4f line;
	double sum = 0;
	vector<Point2i> *ps[] = {&B_left_points, &B_right_points, &B_down_points};
	vector<Point2i> **p = ps;
	for (int i = 0; i < 3; i++)
	{
		fitLine(*(*p), line, DIST_L2, 0, 0.01, 0.01); //�õ��㼯���������
		double cos_theta = line[0];
		double sin_theta = line[1];
		double x0 = line[2], y0 = line[3];
		double k = sin_theta / cos_theta;
		double b = y0 - k * x0;
		sum += getPointToLineError(k, b, *(*(p++))); //�õ��㼯�����ֱ�ߵ����
	}
	return sum;
}

/**
ͨ������õ����ʵ�kֵ
*/
void getK()
{
	//double min_error = 1000000; //-0.00006
	//for (int s1 = 10; s1 < 80; s1++)
	//{
	//	for (int s2 = 10; s2 < 80; s2++)
	//	{
	//		if(s1==35&&s2==35)
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
	//cout << "����k1:" << k1 << endl;
	//cout << "����k2:" << k2 << endl;
	//imshow("jjj", B_picture);
	////show_pictures_name.push_back("Ͱ��ʧ�滹ԭͼ��");
	////show_pictures.push_back(B_picture);
	int s1 = 0, s2 = 0;
	while (true)
	{

	}
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
			P_picture.at<uchar>(x, y) = B_picture.at<uchar>(i, j);
		}
	}
	show_pictures_name.push_back("У�����");
	show_pictures.push_back(P_picture);
}