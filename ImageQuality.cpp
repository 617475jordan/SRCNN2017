#include "ImageQuality.h"

int  imageQuality::imageQualityAssessment(Mat referenceImage, Mat sourceImage)
{
	Mat grayReference;
	Mat graySource;
	cvtColor(referenceImage, grayReference, CV_RGB2GRAY);
	cvtColor(sourceImage, graySource, CV_RGB2GRAY);
	referenceImage = grayReference;
	sourceImage = graySource;
	if (referenceImage.empty() || sourceImage.empty())
	{
		cout << "out" << endl;
		return -1;

	}
	ImageQualityAssessment ssim = ImageQualityAssessment(referenceImage, sourceImage);
	clock_t ss = clock();
	old_ssim = ssim.ssim();
	cout << "ssim: " << old_ssim << endl;

	clock_t ee = clock();
	old_ssim_cost = (double)(ee - ss) / 1000;
	cout << "old_ssim_cost: " << old_ssim_cost << "s" << endl;

	ss = clock();
	 psnr = ssim.psnr();
	cout << "psnr: " << psnr << endl;

	ee = clock();
	psnr_cost = (double)(ee - ss) / 1000;
	cout << "psnr_cost: " << psnr_cost << "s" << endl;
	ssim.blockSize = 11;

	ss = clock();
	new_ssim = ssim.ssim(referenceImage, sourceImage);
	cout << "new_ssim: " << new_ssim << endl;

	ee = clock();
	new_ssim_cost = (double)(ee - ss) / 1000;
	cout << "new_ssim_cost: " << new_ssim_cost << "s" << endl;

	Hash = ssim.hashDistance();
	Dhash = ssim.DhashDistance();
	Phash = ssim.PhashDistance();
	
	aplofIAQ aplofIAQ;
	Iqa = aplofIAQ.IQA(referenceImage, sourceImage);
	win_ssim = aplofIAQ.blockIQA(referenceImage, sourceImage, 2);
    Nc = ssim.Nc(sourceImage, referenceImage);

	cout << "Hash: " << Hash << endl;
	cout << "Dhash: " << Dhash << endl;
	cout << "Phash: " << Phash << endl;
	cout << "IQA: " << Iqa << endl;
	cout << "win_ssim: " << win_ssim << endl;
	cout << "Nc: " << Nc << endl;
	readData();
	writeData();
	return 0;
}

void imageQuality::readData()
{
	FileStorage file;
	file.open("data//data.xml", FileStorage::READ);
	if (!file.isOpened())
	{
		cout << "Data is failed" << endl;
		cout << "Data is initial" << endl;
		dataNum = 1;
	}
	else
	{
		file["dataNum"] >> dataNum;
	}
	file.release();
}

void imageQuality::writeData()
{
	cout << "Saving data..." << endl;
	FileStorage file;
	char fileName[100];
	sprintf(fileName, "data//IQAData%d.xml", dataNum);
	file.open(fileName, FileStorage::WRITE);
	//file << "Compare_PS_with_Origin";
	file << "old_ssim" << old_ssim;
	file << "old_ssim_cost" << old_ssim_cost;
	file << "new_ssim" << new_ssim;
	file << "new_ssim_cost" << new_ssim_cost;
	file << "Hash" << Hash;
	file << "Dhash" << Dhash;
	file << "Phash" << Phash;
	file << "IQA" << Iqa;
	file << "win_ssim" << win_ssim;
	file << "Nc" << Nc;
	file.release();
	cout << "Data is finished" << endl;
	dataNum++;

	file.open("data//data.xml", FileStorage::WRITE);
	file << "dataNum" << dataNum ;
	file.release();
	
}