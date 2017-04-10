#ifndef _IMAGEQULIATY_H_
#define _IMAGEQULIATY_H_
#include "ImageQualityAssessment.h"
#include "APIofIQA.h"
#include <ctime>

class imageQuality
{
public:
	int  imageQualityAssessment(Mat referenceImage, Mat sourceImage);
	void readData();
	void writeData();
public:
	int dataNum = 1;
	double old_ssim;
	double old_ssim_cost;
	double psnr;
	double psnr_cost;
	double new_ssim;
	double new_ssim_cost;
	double Hash;
	double Dhash;
	double Phash;
	double Iqa;
	double win_ssim;
	double Nc;
};
#endif
