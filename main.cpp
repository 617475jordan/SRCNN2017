/*
The core algorithm is in "dehazing.cpp," "guidedfilter.cpp," and "transmission.cpp".
*/
#include "mainhead.h"

Mat temp0, temp1;
Mat out;
void splitAndmerge();

int main()
{
	char filename[photoNum];
	char outname[photoNum];
	mainHead mainHead;
	for (int i = 10; i <= photoNum+10; i++)
	{
		sprintf(filename, inputString, i);
		Mat image0;

		double k = 1.0;
		/*********����ͼƬ*****************/
		image0 = imread(filename);
		if (image0.empty())
		{
			cout << "Image is failed" << endl;
			break;
		}

		if (image0.cols > 640 || image0.rows > 480)
		{
			k = 640.0 / image0.cols;
			cout << "k:" << k << endl;

			int width = image0.cols*k;
			int height = image0.rows*k;
			Size imageSize = Size(width, height);
			resize(image0, image0, imageSize);

		}
		imshow("src", image0);
		/*****removing haze and image Enhancement*/
		imageEnhancement imageEnhancement;
		image0=imageEnhancement.SimplestCB(image0, 1);
		

		

		/*************nonlocalMeansFilter***********************/
	//	deNoising denoising;
	//	out = denoising.denoisingMain(out, 3.0);

		/************super-solution(SRCNN)**********************/
		out = mainHead.Srcnn(image0, scale);
		sprintf(outname, outputString, i);
		imwrite(outname, out);
		namedWindow("out", WINDOW_AUTOSIZE);
		//namedWindow("origin", WINDOW_AUTOSIZE);
		imshow("out", out);
		//imshow("origin", image2);


		/***********ImageQualityAssessment***********/
		//imageQuality ImageQuality;
		//ImageQuality.imageQualityAssessment(image2, out);
		waitKey(0);
		destroyAllWindows();
		image0.release();
		out.release();
	}
}

void splitAndmerge()
{
	mainHead mainHead;
	out = mainHead.compute(temp0);
	/***********ǰ���뱳�����зָ�***********/
	for (int k = 0; k < out.rows; k++)
	{
		for (int j = 0; j < out.cols; j++)
		{
			if (out.at<uchar>(k, j)>0)
			{
				temp1.at<Vec3b>(k, j) = Vec3b(0, 0, 0);
			}
			else
			{
				temp0.at<Vec3b>(k, j) = Vec3b(0, 0, 0);
			}
		}
	}
	namedWindow("temp0", WINDOW_AUTOSIZE);
	namedWindow("temp1", WINDOW_AUTOSIZE);
	imshow("temp0", temp0);
	imshow("temp1", temp1);
	//Mat out1;
	//out1 = mainHead.convert(temp1);
	Mat tmp;
	bitwise_or(out, temp1, tmp);
	out = tmp;
	imwrite("dst//out.png", out);
}
Mat mainHead::compute(Mat image0)
{
	/**********�Աȶȱ���֮��ɫͼ��ȥɫ�㷨*****/
	Mat out;
	rtcprgb2Gray rtcprgb2Gray;
	out = rtcprgb2Gray.rtcprgb2gray(image0);

	/**************���������㷨****************/
	regionGrowing RegionGrowing;
	out = RegionGrowing.RegionGrow(out, 140, 1.0);
	Mat tem;
	morphologyEx(out, tem, MORPH_CLOSE, elemet);
	out = tem;

	/*********�����ͨ�������****************/
	vector<vector<Point>> contours;
	vector<vector<Point>> filterContours;
	contours.clear();
	filterContours.clear();
	findContours(out, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	/*********Ѱ�������ͨ��************/
	double maxArea = 0;
	//int flagMaxArea=0;
	vector<vector<Point>> maxContour;
	for (size_t a = 0; a < contours.size(); a++)
	{
		double area = cv::contourArea(contours[a]);
		if (area > maxArea)
		{
			maxArea = area;
			filterContours.push_back(contours[a]);

		}
	}

	drawContours(out, filterContours, -1, Scalar(255, 255, 255), CV_FILLED);
	return out;
}
/************super-solution**********************/
Mat mainHead::Srcnn(Mat image, double UP_SCALE)
{
	/*��ȡ����ʾԭʼͼ��*/
	IplImage* pImgOrigin;

	pImgOrigin = &(IplImage)image;
	cout << "Read Image Successfully..." << endl;
	clock_t startTime = clock();
	/*��BGR�ռ�ת����YCbCr�ռ�*/
	IplImage* pImgYCbCr = cvCreateImage(cvGetSize(pImgOrigin), IPL_DEPTH_8U, 3);
	cvCvtColor(pImgOrigin, pImgYCbCr, CV_BGR2YCrCb);

	/*����Y-Cb-Cr��ͨ��������ʾYͨ��*/
	IplImage* pImgY = cvCreateImage(cvGetSize(pImgYCbCr), IPL_DEPTH_8U, 1);
	IplImage* pImgCb = cvCreateImage(cvGetSize(pImgYCbCr), IPL_DEPTH_8U, 1);
	IplImage* pImgCr = cvCreateImage(cvGetSize(pImgYCbCr), IPL_DEPTH_8U, 1);
	cvSplit(pImgYCbCr, pImgY, pImgCb, pImgCr, 0);
	cout << "Spliting the Y-Cb-Cr Channel..." << endl;

	/*��Yͨ������2��˫���β�ֵ������ʾͼƬ*/
	IplImage* pImg = cvCreateImage(cvSize(UP_SCALE * pImgY->width, UP_SCALE * pImgY->height), IPL_DEPTH_8U, 1);
	cvResize(pImgY, pImg, CV_INTER_CUBIC);

	/*��һ�����*/
	IplImage* pImgConv1[CONV1_FILTERS];
	for (int i = 0; i < CONV1_FILTERS; i++)
	{
		pImgConv1[i] = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_16U, 1);
		Convolution99(pImg, pImgConv1[i], weights_conv1_data[i], biases_conv1[i]);
		cout << "Convolution Layer I : " << setw(2) << i + 1 << "/64 Cell Complete..." << endl;
	}

	/*�ڶ������*/
	IplImage* pImgConv2[CONV2_FILTERS];
	for (int i = 0; i < CONV2_FILTERS; i++)
	{
		pImgConv2[i] = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_16U, 1);
		Convolution11(pImgConv1, pImgConv2[i], weights_conv2_data[i], biases_conv2[i]);
		cout << "Convolution Layer II : " << setw(2) << i + 1 << "/32 Cell Complete..." << endl;
	}

	/*���������*/
	IplImage* pImgConv3 = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 1);
	Convolution55(pImgConv2, pImgConv3, weights_conv3_data, biases_conv3);
	cout << "Convolution Layer III : 100% Complete..." << endl;

	/*�ϳ����*/
	IplImage* pImgCb2 = cvCreateImage(cvSize(UP_SCALE * pImgCb->width, UP_SCALE * pImgCb->height), IPL_DEPTH_8U, 1);
	cvResize(pImgCb, pImgCb2, CV_INTER_CUBIC);
	IplImage* pImgCr2 = cvCreateImage(cvSize(UP_SCALE * pImgCr->width, UP_SCALE * pImgCr->height), IPL_DEPTH_8U, 1);
	cvResize(pImgCr, pImgCr2, CV_INTER_CUBIC);
	IplImage* pImgYCbCrOut = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 3);
	cvMerge(pImgConv3, pImgCb2, pImgCr2, 0, pImgYCbCrOut);
	IplImage* pImgBGROut = cvCreateImage(cvGetSize(pImg), IPL_DEPTH_8U, 3);
	cvCvtColor(pImgYCbCrOut, pImgBGROut, CV_YCrCb2BGR);
	//cvSaveImage("output//srcnn3.png", pImgBGROut);
	cout << "Reconstruction Complete..." << endl;
	cout << "SRCNN time: " << clock() - startTime << "ms" << endl;
	Mat out(pImgBGROut);
	return out;
}






