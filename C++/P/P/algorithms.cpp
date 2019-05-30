#include "include.h"

using namespace cv;
using namespace std;

PointCount point_count[60][80];								  //小图坐标映射到大图坐标的时候用
vector<Point2i> B_left_points, B_right_points, B_down_points; //存放经过桶形校正后的边界点
int k1 = 0, k2 = 0;											  //桶形校正的参数
int B_picture_size[] = {75, 110};							  //存放新的B图的大小
int B_picture_size_old[] = {75, 110};						  //初始化B图的大小
Mat B_to_P;													  //B图到P图的转化矩阵
Mat B_picture;												  //存放B图
bool fuck_K = false;										  //如果B图大小重构成功为false，重构失败为true

/*************************************************这操蛋的B图坐标转化***********************************************************************/
int getBToOCoordinate(double s1, double s2, int x, int y, int type)
{
	if (type == 0)
	{
		double temp = (1 + s1 * x * x + s2 * y * y) * x;
		return temp > 0 ? (int)(temp + 0.5) : (int)(temp - 0.5);
	}
	double temp = (1 + s1 * x * x + s2 * y * y) * y;
	return temp > 0 ? (int)(temp + 0.5) : (int)(temp - 0.5);
}

/*************************************************这操蛋的B图大小处理***********************************************************************/
/**
检测行数是row列数是col的B图片是否能完全包含O图点
*/
bool checkBSizeIsOk(int row, int col)
{
	bool visit[60][80];
	int num = 0;
	memset(visit, false, sizeof(visit));
	int L_num = row;
	int C_num = col;
	int half_L = L_num / 2;
	int half_C = C_num / 2;
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = getBToOCoordinate(-0.000001 * k1, -0.000001 * k2, x, y, 0);
			int y0 = getBToOCoordinate(-0.000001 * k1, -0.000001 * k2, x, y, 1);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80 && visit[x0][y0] == false)
			{
				visit[x0][y0] = true;
				num++;
			}
		}
	}
	return num == 4800 ? true : false;
}

/**
当K值确定后，得到最小B图
*/
bool updateBSize()
{
	for (int i = 60; i < 100; i++)
	{
		for (int j = 80; j < 120; j++)
		{
			//cout << "操蛋 " << i << " " << j << endl;
			if (checkBSizeIsOk(i, j) == true && checkBSizeIsOk(i - 1, j) == false && checkBSizeIsOk(i, j - 1) == false)
			{
				//cout << "好的 " << i << " " << j << endl;
				//printf("%d %d\n", i, j);
				printf("Update B picture size successful!! Size set as %d %d\n", i, j);
				B_picture_size[0] = i;
				B_picture_size[1] = j;
				return true;
			}
		}
	}
	printf("Can not find a new good size. Size set as %d %d\n", B_picture_size_old[0], B_picture_size_old[1]);
	fuck_K = true;
	//B_picture_size[0] = 84;
	//B_picture_size[1] = 110;
	return false;
}

/*************************************************写他妈的这操蛋的程序*****************************************************/
//void fuckYou()
//{
//	for (k2 = 0; k2 < 200; k2++)
//	{
//		if (updateBSize())
//		{
//			cout << k2 << endl;
//		}
//	}
//}

/*************************************************B图的处理***********************************************************************/
/**
得到对应K值下桶形失真边缘校正后的坐标
因为边缘点都处在图片比较中心的位置，所以只得到边缘点的新坐标可以先假设一个B图的大小
*/
void getNewPoint(double s1, double s2)
{
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
			int x0 = getBToOCoordinate(s1, s2, x, y, 0);
			int y0 = getBToOCoordinate(s1, s2, x, y, 1);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
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

/**
用对应的K值产生B图
*/
void getBPicture()
{
	B_picture = Mat::zeros(B_picture_size[0], B_picture_size[1], CV_8UC1);
	int L_num = B_picture_size[0];
	int C_num = B_picture_size[1];
	//Mat B_picture = Mat::zeros(L_num, C_num, CV_8UC1);
	int half_L = L_num / 2;
	int half_C = C_num / 2;
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = getBToOCoordinate(-0.000001 * k1, -0.000001 * k2, x, y, 0);
			int y0 = getBToOCoordinate(-0.000001 * k1, -0.000001 * k2, x, y, 1);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
				uchar temp = src(x0, y0) == '0' ? 100 : 160;
				B_picture.at<uchar>(i, j) = temp;
			}
		}
	}
	//imshow("ddd", B_picture);
	show_pictures_name.push_back("桶形校正结果");
	show_pictures.push_back(getBigPicture(B_picture, B_picture.rows * 4, B_picture.cols * 4));
}

/**
通过计算得到合适的k值,并显示对应的B图
*/
void getK()
{
	int s1 = 0, s2 = 0;
	while (true)
	{
		getNewPoint(-0.000001 * s1, -0.000001 * s2); //得到对应K值下的边缘点坐标
		int size1 = (int)B_down_points.size();
		int x1 = B_down_points[0].x;
		int x2 = B_down_points[size1 / 2].x;
		int x3 = B_down_points[size1 - 1].x;
		if ((x1 - x2) * (x2 - x3) >= 0)
		{
			break;
		}
		s2++;
	}
	while (true)
	{
		getNewPoint(-0.000001 * s1, -0.000001 * s2);
		int size1 = (int)B_right_points.size();
		int x1 = B_right_points[0].x;
		int x2 = B_right_points[size1 / 2].x;
		int x3 = B_right_points[size1 - 1].x;
		int y1 = B_right_points[0].y;
		int y2 = B_right_points[size1 / 2].y;
		int y3 = B_right_points[size1 - 1].y;
		if ((x1 - x2) / (y1 - y2) >= (x2 - x3) / (y2 - y3))
		{
			break;
		}
		s1++;
	}
	k1 = s1;
	k2 = s2;
	updateBSize();
	getBPicture();
	printf("K1:%d K2:%d \n", k1, k2);
	//cout << "最优k1:" << k1 << endl;
	//cout << "最优k2:" << k2 << endl;
}

/*************************************************P图的处理***********************************************************************/

/**
得到并显示P图
*/
void getPPicture()
{
	float P_picture_size = 100;
	vector<Point2f> B_points(4);
	//因为B图的size可能被改变，所以这里要更新坐标。比如在旧图中23,12代表左上角边缘点，因为新图变大，左上角的坐标也会改变
	int x = B_left_points[0].x - B_picture_size_old[0] / 2 + B_picture_size[0] / 2;
	int y = B_left_points[0].y - B_picture_size_old[1] / 2 + B_picture_size[1] / 2;
	B_points[0] = Point(x, y);
	x = B_left_points[B_left_points.size() - 1].x - B_picture_size_old[0] / 2 + B_picture_size[0] / 2;
	y = B_left_points[B_left_points.size() - 1].y - B_picture_size_old[1] / 2 + B_picture_size[1] / 2;
	B_points[1] = Point(x, y);
	x = B_right_points[B_right_points.size() - 1].x - B_picture_size_old[0] / 2 + B_picture_size[0] / 2;
	y = B_right_points[B_right_points.size() - 1].y - B_picture_size_old[1] / 2 + B_picture_size[1] / 2;
	B_points[2] = Point(x, y);
	x = B_right_points[0].x - B_picture_size_old[0] / 2 + B_picture_size[0] / 2;
	y = B_right_points[0].y - B_picture_size_old[1] / 2 + B_picture_size[1] / 2;
	B_points[3] = Point(x, y);
	vector<Point2f> P_points(4);
	P_points[0] = Point2f(0, 0);
	P_points[1] = Point2f(P_picture_size, 0);
	P_points[2] = Point2f(P_picture_size, P_picture_size);
	P_points[3] = Point2f(0, P_picture_size);

	B_to_P = getPerspectiveTransform(B_points, P_points);
	vector<Point2f> point, point_site_in_new_picture;
	for (int i = 0; i < B_picture_size[0]; i++)
	{
		for (int j = 0; j < B_picture_size[1]; j++)
		{
			point.push_back(Point2f((float)i, (float)j));
		}
	}
	perspectiveTransform(point, point_site_in_new_picture, B_to_P);
	int row_max = -1;
	int row_min = 20000;
	int col_max = -1;
	int col_min = 20000;
	/*去除B图的前20行，然后在P图中显示*/
	for (int i = 20 * B_picture_size[1]; i < point_site_in_new_picture.size(); i++)
	{
		int x = (int)point_site_in_new_picture[i].x;
		int y = (int)point_site_in_new_picture[i].y;
		if (x > row_max)
			row_max = x;
		if (x < row_min)
			row_min = x;
		if (y > col_max)
			col_max = y;
		if (y < col_min)
			col_min = y;
	}
	//确保去除掉前20行后的B图坐标都为正
	for (int i = 0; i < point_site_in_new_picture.size(); i++)
	{
		point_site_in_new_picture[i].x -= row_min - 1;
		point_site_in_new_picture[i].y -= col_min - 1;
	}
	Mat P_picture = Mat::zeros(row_max - row_min + 2, col_max - col_min + 2, CV_8UC1);
	for (int i = 20; i < B_picture_size[0]; i++) //去除B图的上面20行后显示在P图，因为放不下
	{
		for (int j = 0; j < B_picture_size[1]; j++)
		{
			int x = (int)point_site_in_new_picture[i * B_picture_size[1] + j].x;
			int y = (int)point_site_in_new_picture[i * B_picture_size[1] + j].y;
			P_picture.at<uchar>(x, y) = B_picture.at<uchar>(i, j);
		}
	}
	//imshow("ddd", P_picture);
	//waitKey();
	show_pictures_name.push_back("校正结果");
	show_pictures.push_back(P_picture);
}

/*************************************************校正表的处理***********************************************************************/

Point2i tidyCoordinate(vector<Point2f> &points)
{
	int row_max = -1;
	int row_min = 20000;
	int col_max = -1;
	int col_min = 20000;
	for (int i = 20; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			int x = (int)points[i * 80 + j].x;
			int y = (int)points[i * 80 + j].y;
			if (x > row_max)
				row_max = x;
			if (x < row_min)
				row_min = x;
			if (y > col_max)
				col_max = y;
			if (y < col_min)
				col_min = y;
		}
	}
	for (int i = 59; i >= 0; i--)
	{
		for (int j = 0; j < 80; j++)
		{
			//if (i < 10)
			//{
			//	points[i * 80 + j].x = 0;
			//	points[i * 80 + j].y = 0;
			//	continue;
			//}
			//if ((i >= 10) && (i < 20))
			//{
			//	points[i * 80 + j].x =points[15*80+j].x;
			//	points[i * 80 + j].y = points[15 * 80 + j].y;
			//}
			points[i * 80 + j].x -= row_min - 1;
			points[i * 80 + j].y -= col_min - 1;
		}
	}
	return Point2i(row_max - row_min + 2, col_max - col_min + 2);
}

bool notZero(int i, int j)
{
	if (i >= 0 && i < 60 && j >= 0 && j < 80 && point_count[i][j].num != 0)
	{
		return true;
	}
	return false;
}

void getTable(int s1, int s2, Mat tran_matrix)
{
	memset(point_count, 0, sizeof(point_count));
	int L_num = B_picture_size[0];
	int C_num = B_picture_size[1];
	//printf("%d %d\n", L_num, C_num);
	//cout <<"K"<< k1 <<" "<< k2<<endl;
	//if (checkBSizeIsOk(L_num, C_num))
	//{
	//	cout << "OK" << endl;
	//}
	//else
	//{
	//	cout << "fuck you!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	//}

	int half_L = L_num / 2;
	int half_C = C_num / 2;
	for (int i = 0; i < L_num; i++)
	{
		int x = i - half_L;
		for (int j = 0; j < C_num; j++)
		{
			int y = j - half_C;
			int x0 = getBToOCoordinate(-0.000001 * s1, -0.000001 * s2, x, y, 0);
			int y0 = getBToOCoordinate(-0.000001 * s1, -0.000001 * s2, x, y, 1);
			x0 += 30;
			y0 += 40;
			if (x0 >= 0 && x0 < 60 && y0 >= 0 && y0 < 80)
			{
				point_count[x0][y0].num++;
				point_count[x0][y0].x += i;
				point_count[x0][y0].y += j;
			}
		}
	}
	if (fuck_K)
	{
		for (int i = 0; i < 60; i++)
		{
			for (int j = 0; j < 80; j++)
			{
				if (point_count[i][j].num == 0)
				{
					bool isOk = false;
					for (int m = -1; m < 2; m++)
					{
						for (int n = -1; n < 2; n++)
						{
							if (notZero(i + m, j + n))
							{
								point_count[i][j] = point_count[i + m][j + n];
								isOk = true;
								break;
							}
						}
					}
					if (isOk == false)
					{
						for (int m = -2; m < 3; m++)
						{
							for (int n = -2; n < 3; n++)
							{
								if (notZero(i + m, j + n))
								{
									point_count[i][j] = point_count[i + m][j + n];
									isOk = true;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	vector<Point2f> B_points(4800);
	int h = 0;
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			//if (point_count[i][j].num == 0)
			//{
			//	B_points[h++] = Point(0, 0);
			//	continue;
			//}
			// if (point_count[i][j].num == 0)
			// {
			// 	cout << "fuck you!!!! " << i << " " << j << endl;
			// }
			B_points[h].x = (float)point_count[i][j].x / point_count[i][j].num;
			B_points[h++].y = (float)point_count[i][j].y / point_count[i][j].num;
		}
	}
	//以上得到的B图坐标是没有误差的（没有对应位置的也找了附近的点，可以认为误差很小）
	vector<Point2f> P_points(4800);
	perspectiveTransform(B_points, P_points, tran_matrix);
	Point2i temp = tidyCoordinate(P_points);
	Mat table_picture = Mat::zeros(temp.x, temp.y, CV_8UC1);
	for (int i = 20; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			int x = (int)P_points[i * 80 + j].x;
			int y = (int)P_points[i * 80 + j].y;
			//if (x < 700 && y < 700 && x >= 0 && y >= 0)
			//{
			table_picture.at<uchar>(x, y) = src.at<uchar>(i, j) == '0' ? 100 : 160;
			/*		}*/
		}
	}
	show_pictures_name.push_back("校正表直接映射");
	show_pictures.push_back(table_picture);

	/*打印校正表*/
	printf("O_to_P_table[60][80]={");
	for (int i = 0; i < 4800; i++)
	{
		if (i % 10 == 0)
		{
			printf("\n");
		}
		int x = (int)P_points[i].x;
		int y = (int)P_points[i].y;
		printf("{%d, %d}", x, y);
		if (i != 4799)
		{
			printf(", ");
		}
	}
	printf("}");
}