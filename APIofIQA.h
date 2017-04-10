#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class aplofIAQ
{
public:
#define PSNR 1
#define CV_SSIM 2
#define MYSSIM 3
#define HASH 4
#define DHASH 5
#define PHASH 6

public:
	double blockIQA(Mat referenceImage, Mat sourceImage, int size, int IQAMethod = CV_SSIM);
	double IQA(Mat referenceImage, Mat sourceImage, int IQAMethod = CV_SSIM);
	double hashDistance(Mat referenceImage, Mat sourceImage);
	double DhashDistance(Mat referenceImage, Mat sourceImage);
	double PhashDistance(Mat referenceImage, Mat sourceImage);
	double psnr(Mat referenceImage, Mat sourceImage);
	double cvSsim(Mat referenceImage, Mat sourceImage);
	double mySsim(Mat referenceImage, Mat sourceImage);
	Mat calcHashCode(Mat src);
	Mat calcPHashCode(Mat src);
	Mat calcDHashCode(Mat src);

};


