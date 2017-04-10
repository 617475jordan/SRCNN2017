#ifndef _DENOISING_H_
#define _DENOISING_H_

#include <opencv2/opencv.hpp>  
#include <iostream>  
#include <time.h>
#include <conio.h>
using namespace cv;
using namespace std;

class deNoising
{
public:
	void addNoiseSoltPepperMono(Mat& src, Mat& dest, double per);
	void addNoiseMono(Mat& src, Mat& dest, double sigma);
	void nonlocalMeansFilter(Mat& src, Mat& dest, int templeteWindowSize, int searchWindowSize, double h, double sigma );
	void addNoise(Mat&src, Mat& dest, double sigma, double sprate);
	Mat denoisingMain(Mat src, double noise_sigma);
};
#endif