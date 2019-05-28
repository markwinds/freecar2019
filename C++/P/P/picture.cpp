#include "include.h"

using namespace cv;
using namespace std;

Mat src = Mat::zeros(60, 80, CV_8UC1);
vector<Point2i> left_points, right_points, down_points;
vector<Mat> show_pictures;
vector<string> show_pictures_name;

/**
从指定的文件读取图片数据
*/
void readFileToSrc()
{
	ifstream fp;
	fp.open("road2.txt");
	for (int i = 0; i < 60; i++)
	{
		uchar *p = src.ptr<uchar>(i);
		for (int j = 0; j < 80; j++)
		{
			fp >> p[j];
		}
	}
	fp.close();
}

/**
输入小图片，返回指定尺寸的大图
*/
Mat getBigPicture(Mat img, int Ln, int Col)
{
	Mat ans = Mat::zeros(Ln, Col, CV_8UC1);
	vector<Point2f> points_img(4);
	vector<Point2f> points_ans(4);
	points_img[0] = Point2f(0, 0);
	points_img[1] = Point2f((float)img.rows, 0);
	points_img[2] = Point2f((float)img.rows, (float)img.cols);
	points_img[3] = Point2f(0, (float)img.cols);
	points_ans[0] = Point2f(0, 0);
	points_ans[1] = Point2f((float)ans.rows, 0);
	points_ans[2] = Point2f((float)ans.rows, (float)ans.cols);
	points_ans[3] = Point2f(0, (float)ans.cols);

	Mat ans_to_img = getPerspectiveTransform(points_ans, points_img);
	vector<Point2f> point, point_site_in_new_picture;
	for (int i = 0; i < ans.rows; i++)
	{
		for (int j = 0; j < ans.cols; j++)
		{
			point.push_back(Point2f((float)i, (float)j));
		}
	}
	perspectiveTransform(point, point_site_in_new_picture, ans_to_img);
	int h = 0;
	for (int i = 0; i < ans.rows; i++)
	{
		uchar *p = ans.ptr<uchar>(i);
		for (int j = 0; j < ans.cols; j++)
		{
			int x = (int)(point_site_in_new_picture[h].x + 0.5);
			int y = (int)(point_site_in_new_picture[h++].y + 0.5);
			if (x >= 0 && y >= 0 && x < img.rows && y < img.cols)
				p[j] = img(x, y);
		}
	}
	return ans;
}

/**
输入小图和大图的尺寸，以大图的形式显示
*/
void showBigPicture(Mat img, int Ln, int Col)
{
	Mat ans = getBigPicture(img, Ln, Col);
	imshow("Big picture", ans);
	waitKey();
}

/**
得到方形的左右下边界放入对应的数组
*/
void getBorder()
{
	int j = 43;
	int i = 32;
	for (i = 32; i > 1; i--)
	{
		if (src(i, j) == '0')
		{
			break;
		}
	}
	i = i + 1;
	int wide = 0;
	int pre_wide = wide;
	while (true)
	{
		pre_wide = wide;
		wide = 0;
		int j = 43;
		while (src(i, j) != '0')
		{
			j--;
		}
		wide -= j;
		left_points.push_back(Point(i, j + 1));
		j = 43;
		while (src(i, j) != '0')
		{
			j++;
		}
		wide += j;
		right_points.push_back(Point(i, j - 1));
		if (wide < pre_wide)
			break;
		i++;
	}
	i--;
	int begin = i;
	left_points.pop_back();
	int left = left_points[left_points.size() - 1].y;
	right_points.pop_back();
	int right = right_points[right_points.size() - 1].y;
	for (int j = left; j <= right; j++)
	{
		i = begin;
		while (src(i, j) != '0')
		{
			i++;
		}
		down_points.push_back(Point(i - 1, j));
	}

	//下面开始显示边界
	Mat border_picture = Mat::zeros(60, 80, CV_8UC1);
	for (int i = 0; i < 60; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			border_picture.at<uchar>(i, j) = src(i, j) == '0' ? 100 : 160;
		}
	}
	for (int i = 0; i < left_points.size(); i++)
	{
		int x = left_points[i].x;
		int y = left_points[i].y;
		border_picture.at<uchar>(x, y) = 255;
	}
	for (int i = 0; i < right_points.size(); i++)
	{
		int x = right_points[i].x;
		int y = right_points[i].y;
		border_picture.at<uchar>(x, y) = 255;
	}
	for (int i = 0; i < down_points.size(); i++)
	{
		int x = down_points[i].x;
		int y = down_points[i].y;
		border_picture.at<uchar>(x, y) = 0;
	}
	show_pictures_name.push_back("边缘处理图片");
	show_pictures.push_back(getBigPicture(border_picture, 60 * 3, 80 * 3));
}

void showPicture()
{
	for (int i = 0; i < show_pictures.size(); i++)
	{
		imshow(show_pictures_name[i], show_pictures[i]);
	}
	waitKey();
}

void savePicture()
{
	for (int i = 0; i < show_pictures.size(); i++)
	{
		imwrite(show_pictures_name[i] + ".jpg", show_pictures[i]);
	}
}
