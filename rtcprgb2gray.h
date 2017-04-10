#ifndef RTCPRGB2GRAY_H
#define RTCPRGB2GRAY_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

class rtcprgb2Gray
{
public:
	Mat rtcprgb2gray(const cv::Mat &im);
    void add(const cv::Vec3b &c0, const cv::Vec3b &c1, std::vector<cv::Vec3f> &P, std::vector<float> &det);

};



#endif
