#ifndef EASYPR_CREATE_DATA_H_
#define EASYPR_CREATE_DATA_H_

#include "opencv2/opencv.hpp"
#include "config.h"

using namespace cv;
using namespace std;

/*! \namespace easypr
Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr
{
	// 获取图像边界颜色的平均值
	int getBoderColor(Mat img);
	// shift an image
	Mat translateImg(Mat img, int offsetx, int offsety, int bk = 0);
	// rotate an image
	Mat rotateImg(Mat source, float angle, int bk = 0);
	// crop the image
	Mat cropImg(Mat src, int x, int y, int shift, int bk = 0);
	// 对给定字符图像进行几何变换(平移/旋转)，从而人工合成得到新的图像，用于训练
	Mat generateSyntheticImage(const Mat& image, int use_swap = 1);

} /*! \namespace easypr*/

#endif  // EASYPR_CREATE_DATA_H_