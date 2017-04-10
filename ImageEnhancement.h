#ifndef _IMAGEENJANCEMENT_H_
#define _IMAGEENJANCEMENT_H_

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

class imageEnhancement
{
public:
	Mat SimplestCB(Mat& in, float percent);
};
#endif