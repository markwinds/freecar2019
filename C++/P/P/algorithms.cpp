#include "include.h"

using namespace cv;
using namespace std;

/**����*/
PointCount point_count[60][80];								  //Сͼ����ӳ�䵽��ͼ�����ʱ����
vector<Point2i> B_left_points, B_right_points, B_down_points; //��ž���Ͱ��У����ı߽��
int k1 = 0, k2 = 0;											  //Ͱ��У���Ĳ���
Mat B_picture;												  //���Bͼ
bool fuck_K = false;										  //���Bͼ��С�ع��ɹ�Ϊfalse���ع�ʧ��Ϊtrue
Point2f relocate_point;
/**����У����ת������*/
Mat B_to_P;													  //Bͼ��Pͼ��ת������
Mat P_to_B;
/**�������ͼƬ�Ĵ�С*/
int P_picture_size[2] = {334, 530};
int B_picture_size[] = { 75, 110 };							  //����µ�Bͼ�Ĵ�С
int B_picture_size_old[] = { 75, 110 };						  //��ʼ��Bͼ�Ĵ�С
/**����ͼ֮��ת����У����*/
vector<Point2f> point_O_B, point_B_P, point_O_P, point_B_O, point_P_B, point_P_O;

/*************************************************��ٵ���Bͼ����ת��***********************************************************************/
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

/*************************************************��ٵ���Bͼ��С����***********************************************************************/
/**
���������row������col��BͼƬ�Ƿ�����ȫ����Oͼ��
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
��Kֵȷ���󣬵õ���СBͼ
*/
bool updateBSize()
{
	for (int i = 60; i < 100; i++)
	{
		for (int j = 80; j < 120; j++)
		{
			//cout << "�ٵ� " << i << " " << j << endl;
			if (checkBSizeIsOk(i, j) == true && checkBSizeIsOk(i - 1, j) == false && checkBSizeIsOk(i, j - 1) == false)
			{
				//cout << "�õ� " << i << " " << j << endl;
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

/*************************************************д�������ٵ��ĳ���*****************************************************/
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

/*************************************************Bͼ�Ĵ���***********************************************************************/
/**
�õ���ӦKֵ��Ͱ��ʧ���ԵУ���������
��Ϊ��Ե�㶼����ͼƬ�Ƚ����ĵ�λ�ã�����ֻ�õ���Ե�������������ȼ���һ��Bͼ�Ĵ�С
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
	//����ͼӳ�䵽Сͼ����ôСͼӳ�䵽��ͼ��������Ǵ�ͼӳ�䵽Сͼ�õ�����е������ƽ��ֵ
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
�ö�Ӧ��Kֵ����Bͼ
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
				point_B_O.push_back(Point(x0, y0));
			}
			else
			{
				point_B_O.push_back(Point(-1, -1));
			}
		}
	}
	//imshow("ddd", B_picture);
	show_pictures_name.push_back("Ͱ��У�����");
	show_pictures.push_back(getBigPicture(B_picture, B_picture.rows * 4, B_picture.cols * 4));
}

/**
ͨ������õ����ʵ�kֵ,����ʾ��Ӧ��Bͼ
*/
void getK()
{
	int s1 = 0, s2 = 0;
	while (true)
	{
		getNewPoint(-0.000001 * s1, -0.000001 * s2); //�õ���ӦKֵ�µı�Ե������
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
	//cout << "����k1:" << k1 << endl;
	//cout << "����k2:" << k2 << endl;
}

/*************************************************Pͼ�Ĵ���***********************************************************************/

/**
�õ�����ʾPͼ
*/
void getPPicture()
{
	float P_size = 60;
	vector<Point2f> B_points(4);
	/**��ΪBͼ��size���ܱ��ı䣬��������Ҫ�������ꡣ�����ھ�ͼ��23,12�������ϽǱ�Ե�㣬��Ϊ��ͼ������Ͻǵ�����Ҳ��ı�*/
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
	P_points[1] = Point2f(P_size, 0);
	P_points[2] = Point2f(P_size, P_size);
	P_points[3] = Point2f(0, P_size);

	/**�õ�ת������*/
	B_to_P = getPerspectiveTransform(B_points, P_points);
	P_to_B = getPerspectiveTransform(P_points, B_points);

	/**��������ȷ��Pͼ�Ĵ�С*/
	vector<Point2f> point, point_site_in_new_picture;
	point.push_back(Point2f(20, 0));
	point.push_back(Point2f((float)B_picture_size[0] - 1, 0));
	point.push_back(Point2f(20,(float)B_picture_size[1] - 1));
	perspectiveTransform(point, point_site_in_new_picture, B_to_P);
	Point2f temp_b,temp_c;
	relocate_point = point_site_in_new_picture[0];
	temp_b = point_site_in_new_picture[1];
	temp_c = point_site_in_new_picture[2];
	P_picture_size[0] = (int)(temp_b.x - relocate_point.x + 100);
	P_picture_size[1] = (int)(temp_c.y - relocate_point.y + 100);
	point.clear();
	point_site_in_new_picture.clear();
	/**�õ�BP��ת����*/
	for (int i = 0; i < B_picture_size[0]; i++)
	{
		for (int j = 0; j < B_picture_size[1]; j++)
		{
			point.push_back(Point2f((float)i, (float)j));
		}
	}
	perspectiveTransform(point, point_B_P, B_to_P);
	for (int i = 0; i < B_picture_size[0] * B_picture_size[1]; i++)
	{
		if (i < 20 * B_picture_size[1])
		{
			point_B_P[i] = Point2f(-1, -1);
		}
		else
		{
			point_B_P[i].x -= (relocate_point.x - 50);
			point_B_P[i].y -= (relocate_point.y - 50);
		}
	}
	/**����PB��*/
	point.clear();
	point_site_in_new_picture.clear();
	for (int i = (int)relocate_point.x - 50; i < P_picture_size[0] + (relocate_point.x - 50); i++)
	{
		for (int j = (int)relocate_point.y - 50; j < P_picture_size[1] + (relocate_point.y - 50); j++)
		{
			point.push_back(Point2f((float)i, (float)j));
		}
	}
	perspectiveTransform(point, point_P_B, P_to_B);
	for (int i = 0; i < P_picture_size[0] * P_picture_size[1]; i++)
	{
		int x = (int)point_P_B[i].x;
		int y = (int)point_P_B[i].y;
		if (!(x >= 0 && y >= 0 && x < B_picture_size[0] && y < B_picture_size[1]))
		{
			point_P_B[i] = Point2f(-1, -1);
		}
	}

	/**����PO��*/
	for (int i = 0; i < P_picture_size[0] * P_picture_size[1]; i++)
	{
		int x = (int)point_P_B[i].x;
		int y = (int)point_P_B[i].y;
		if (!(x >= 0 && y >= 0 && x < B_picture_size[0] && y < B_picture_size[1]))
		{
			point_P_O.push_back(Point2f(-1, -1));
		}
		else
		{
			point_P_O.push_back(Point2f(point_B_O[x*B_picture_size[1]+y].x, point_B_O[x * B_picture_size[1] + y].y));
		}
	}

	/**��ʾ����У�����*/
	Mat P_picture = Mat::zeros(P_picture_size[0], P_picture_size[1], CV_8UC1);
	//printf("%d %d\n", P_picture_size[0], P_picture_size[1]);
	for (int i = 20; i < B_picture_size[0]; i++) //ȥ��Bͼ������20�к���ʾ��Pͼ����Ϊ�Ų���
	{
		for (int j = 0; j < B_picture_size[1]; j++)
		{
			int x = (int)point_B_P[i * B_picture_size[1] + j].x;
			int y = (int)point_B_P[i * B_picture_size[1] + j].y;
			//printf("%d %d\n", x,y);
			P_picture.at<uchar>(x, y) = B_picture.at<uchar>(i, j);
		}
	}
	//imshow("ddd", P_picture);
	//waitKey();
	show_pictures_name.push_back("У�����");
	show_pictures.push_back(P_picture);
}

/*************************************************У����Ĵ���***********************************************************************/

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
	/**��Bͼӳ�䵽Oͼ���õ�OB��*/
	memset(point_count, 0, sizeof(point_count));
	int L_num = B_picture_size[0];
	int C_num = B_picture_size[1];
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
	/*�õ�OB��*/
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			float x= (float)point_count[i][j].x / point_count[i][j].num;
			float y= (float)point_count[i][j].y / point_count[i][j].num;
			point_O_B.push_back(Point2f(x, y));
		}
	}
	/**����OP��*/
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			int x = (int)point_O_B[i * 80 + j].x;
			int y = (int)point_O_B[i * 80 + j].y;
			int x1 = (int)point_B_P[x * B_picture_size[1] + y].x;
			int y1 = (int)point_B_P[x * B_picture_size[1] + y].y;
			point_O_P.push_back(Point2f((float)x1,(float)y1));
		}
	}
	Mat table_picture = Mat::zeros(P_picture_size[0], P_picture_size[1], CV_8UC1);
	for (int i = 20; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			int x = (int)point_O_P[i * 80 + j].x;
			int y = (int)point_O_P[i * 80 + j].y;
			table_picture.at<uchar>(x, y) = src.at<uchar>(i, j) == '0' ? 100 : 160;
		}
	}
	show_pictures_name.push_back("У����ֱ��ӳ��");
	show_pictures.push_back(table_picture);

	

	//vector<Point2f> point1, point2;
	//for (int i = -400; i < 500 - 400; i++)
	//{
	//	for (int j = -200; j < 500 - 200; j++)
	//	{
	//		point1.push_back(Point(i, j));
	//	}
	//}
	//perspectiveTransform(point1, point2, P_to_B);
	//for (int i = 0; i < 500; i++)
	//{
	//	for (int j = 0; j < 500; j++)
	//	{
	//		int x = (int)point2[i * 500 + j].x;
	//		int y = (int)point2[i * 500 + j].y;
	//		if (x >= 0 && y >= 0 && x < B_picture_size[0] && y < B_picture_size[1])
	//		{
	//			point2[i * 500 + j].x = point_B_O[x * B_picture_size[1] + y].x;
	//			point2[i * 500 + j].y = point_B_O[x * B_picture_size[1] + y].y;
	//		}
	//		else
	//		{
	//			point2[i * 500 + j].x = -1;
	//			point2[i * 500 + j].y = -1;
	//		}
	//	}
	//}

	/*��ӡУ����*/
	//printf("int16 O_to_P_table[60][80][2]={");
	//for (int i = 0; i < 4800; i++)
	//{
	//	if (i % 10 == 0)
	//	{
	//		printf("\n");
	//	}
	//	int x = (int)P_points[i].x;
	//	int y = (int)P_points[i].y;
	//	/*x = (abs(x) > 1000) ? 0 : x;
	//	y = (abs(y) > 1000) ? 0 : y;*/
	//	printf("%d, %d", x, y);
	//	if (i != 4799)
	//	{
	//		printf(", ");
	//	}
	//}
	//printf("};");

	//ofstream fp("P_O_table.cpp");
	//cout << "Begin write P_O_table.cpp" << endl;
	//fp << "#include \"include.h\"" << endl;
	//fp << "int P_O_table[" << P_picture_size[0] << "][" << P_picture_size[1] << "][2]={"<<endl;
	//for (int i = 0; i < P_picture_size[0]*P_picture_size[1]; i++)
	//{
	//	int x = (int)point_P_O[i].x;
	//	int y = (int)point_P_O[i].y;
	//	fp << x << "," << y;
	//	if (i < P_picture_size[0] * P_picture_size[1] - 1)
	//		fp << ",";
	//}
	//fp<<"};";
	//cout << "P_O_table.cpp writed successful!" << endl;
	//fp.close();

	//ofstream fp("O_P_table.cpp");
	//cout << "Begin write O_P_table.cpp" << endl;
	//fp << "#include \"include.h\"" << endl;
	//fp << "int O_P_table[" <<60 << "][" << 80 << "][2]={" << endl;
	//for (int i = 0; i < 4800; i++)
	//{
	//	int x = (int)point_O_P[i].x;
	//	int y = (int)point_O_P[i].y;
	//	fp << x << "," << y;
	//	if (i < 4800 - 1)
	//		fp << ",";
	//}
	//fp << "};";
	//cout << "O_P_table.cpp writed successful!" << endl;
	//fp.close();

	Mat temp_picture = Mat::zeros(P_picture_size[0], P_picture_size[1], CV_8UC1);
	for (int i = 0; i < P_picture_size[0]; i++)
	{
		for (int j = 0; j < P_picture_size[1]; j++)
		{
			int x = (int)(point_P_O[i * P_picture_size[1] + j].x);
			int y = (int)(point_P_O[i * P_picture_size[1] + j].y);
			if(x>0)
			temp_picture.at<uchar>(i, j) = src(x,y)=='0'?100:200;
		}
	}
	show_pictures_name.push_back("PO");
	show_pictures.push_back(temp_picture);
}