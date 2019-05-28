#include "include.h"
using namespace cv;
using namespace std;
int main()
{

    /*******************测试读取图片****************/
    // readFileToSrc();
    // showPicture(src[0], 60, 80);

    /*******************测试桶形变换设置的参数****************/
     //readFileToSrc();
     //k1 = -0.00002;
     //k2 = -0.00006;
     //setK1K2ForPicture1();
     //showPicture(picture1[0], 60, 80);

    /*******************测试最小校正表****************/
    //readFileToSrc();
    //for (int i = 60; i < 100; i++)
    //{
    //	for (int j = 80; j < 200; j++)
    //	{
    //		if (updatePicture(i, j) == 4800 && updatePicture(i - 1, j) != 4800 && updatePicture(i, j - 1) != 4800)
    //		{
    //			MyPoint temp;
    //			temp.x = i;
    //			temp.y = j;
    //			ans.push_back(temp);
    //		}
    //	}
    //}
    //for (unsigned int i = 0; i < ans.size(); i++)
    //{
    //	cout << ans[i].x << ", " << ans[i].y << endl;
    //}

    /*******************查看生成的P图****************/

    //   readFileToSrc();
    //   getBPicture();
    //showPPicture();

    /*******************查看由O图直接映射生成的稀疏P图****************/
    //showOPPicture();

    /*******************打印出映射表****************/
    //showTable();

    waitKey();
    return 0;
}