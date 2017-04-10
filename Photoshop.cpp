#include "Photoshop.h"

Mat photoShop::Zoom(Mat& img, float fx, float fy)
{
	int height = (int)(img.rows*fy + 0.5);
	int width = (int)(img.cols*fx + 0.5);
	Mat dst;
	if (dst.empty())
		dst.create(height, width, img.type());

	dst = cv::Scalar::all(0);

	int i, j, i0, j0, k;
	Size size = dst.size();
	int chns = dst.channels();

	for (i = 0; i<size.height; ++i)
	{
		i0 = (int)(i / fy + 0.5); // 在原图的位置行      

		const unsigned char* srcData = (const unsigned char*)img.data + img.step*i0;
		unsigned char* dstData = (unsigned char*)dst.data + dst.step*i;

		for (j = 0; j<size.width; ++j)
		{
			j0 = (int)(j / fx + 0.5);  // 在原图的列  
			if (i0 >= 0 && i0<img.rows && j0 >= 0 && j0<img.cols)
			{
				for (k = 0; k<chns; ++k)
				{
					dstData[j*chns + k] = srcData[j0*chns + k];
					//setPixel(dst, i, j, k, getPixel(img, i0, j0, k));  
				}
			}
			else
			{
				for (k = 0; k<chns; ++k)
				{
					dstData[j*chns + k] = 255;
					//setPixel(dst, i, j, k, 255);  
				}
			}
		}
	}
	return dst;
}