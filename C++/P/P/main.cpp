#include "include.h"
using namespace cv;
using namespace std;
int main()
{
	readFileToSrc();
	getBorder();
	getK();
	getPPicture();
	getTable();
	savePicture();
	showPicture();
	return 0;
}