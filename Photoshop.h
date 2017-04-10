#ifndef _PHOTOSHOP_H_
#define _PHOTOSHOP_H_
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
using namespace cv;
using namespace std;

class photoShop
{
public:
	Mat Zoom(Mat& img, float fx, float fy);
};
#endif