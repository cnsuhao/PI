#ifndef EASYPR_CORE_COREFUNC_H_
#define EASYPR_CORE_COREFUNC_H_


#include "opencv2/opencv.hpp"
#include "core/plate.hpp"
#include "core/character.hpp"
#include "basedef.h"

using namespace cv;
using namespace std;

/*! \namespace easypr
Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr
{

	// 和给定的颜色匹配的像素置为255，否则置为0，从而将图像二值化
	Mat colorMatch(const Mat& src, Mat& match, const Color r, const bool adaptive_minsv);

	//Mat mserMatch(const Mat& src, Mat& match, const Color r,
	//  std::vector<RotatedRect>& plateRect, std::vector<Rect>& out_charRect);

	// 判断车牌颜色是否为该颜色
	bool plateColorJudge(const Mat& src, const Color r, const bool adaptive_minsv, float& percent);

	// 寻找左右边界
	bool bFindLeftRightBound(Mat& bound_threshold, int& posLeft, int& posRight);
	// 寻找左右边界
	bool bFindLeftRightBound1(Mat& bound_threshold, int& posLeft, int& posRight);
	// 寻找左右边界
	bool bFindLeftRightBound2(Mat& bound_threshold, int& posLeft, int& posRight);

	// 将车牌上下边缘裁剪掉
	void clearBorder(const Mat &img, Rect& cropRect);
	
	// 将柳钉所在行的像素值全部置为0
	void clearLiuDingOnly(Mat& img);
	// 用跳变次数来判断是否为车牌，去除柳钉所在行的像素值置为0
	bool clearLiuDing(Mat& img);
	// 去除柳钉并且寻找上下边界
	void clearLiuDing(Mat mask, int& top, int& bottom);
	// 将柳钉所在行像素置为0和并裁剪掉上下边缘
	Mat clearLiuDingAndBorder(const Mat& grayImage, Color color);

	// 根据车牌图像，计算得到车牌颜色
	EASYPR_API Color getPlateType(const Mat& src, const bool adaptive_minsv);
	// 对图像进行直方图均衡化处理
	Mat histeq(Mat in);
	Rect GetCenterRect(Mat& in);
	Mat CutTheRect(Mat& in, Rect& rect);
	// 利用otsu算法计算图像二值化阈值
	int ThresholdOtsu(Mat mat);
	// project histogram
	Mat ProjectedHistogram(Mat img, int t, int threshold = 20);

	Mat showHistogram(const Mat& hist);

	Mat preprocessChar(Mat in, int char_size);

	Rect GetChineseRect(const Rect rectSpe);

	bool verifyCharSizes(Rect r);
	bool verifyPlateSize(Rect mr);
	bool verifyRotatedPlateSizes(RotatedRect mr, bool showDebug = false);

	// non-maximum suppression
	void NMStoCharacter(std::vector<CCharacter> &inVec, double overlap);
	void NMStoCharacterByRatio(std::vector<CCharacter> &inVec, double overlap, const Rect groundRect);
	// draw rotatedRectangle
	void rotatedRectangle(InputOutputArray img, RotatedRect rect,
		const Scalar& color, int thickness = 1,
		int lineType = LINE_8, int shift = 0);

	// 为了防止光照不均的影响，该二值化方法是通过将图像分成多个块，每个块单独进行二值化
	void spatial_ostu(InputArray _src, int grid_x, int grid_y, Color type = BLUE);

	// 当图像尺寸超过maxSize时，将图像尺寸缩放到maxSize范围内
	Mat scaleImage(const Mat& image, const Size& maxSize, double& scale_ratio);

	// Scale back RotatedRect
	RotatedRect scaleBackRRect(const RotatedRect& rr, const float scale_ratio);

	//! use verify size to first generate char candidates
	void mserCharMatch(const Mat &src, std::vector<Mat> &match, std::vector<CPlate>& out_plateVec_blue, std::vector<CPlate>& out_plateVec_yellow,
		bool usePlateMser, std::vector<RotatedRect>& out_plateRRect_blue, std::vector<RotatedRect>& out_plateRRect_yellow, int index = 0, bool showDebug = false);

	// computer the insert over union about two rrect
	bool computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height, const float thresh, float& result);
	float computeIOU(const RotatedRect& rrect1, const RotatedRect& rrect2, const int width, const int height);

	bool computeIOU(const Rect& rect1, const Rect& rect2, const float thresh, float& result);
	float computeIOU(const Rect& rect1, const Rect& rect2);

	/** @brief convert form mser point to image.

	The function created first by Hailiang Xu.
	Modified by Ruoze Liu.

	@param
	*/
	Mat adaptive_image_from_points(const std::vector<Point>& points,
		const Rect& rect, const Size& size, const Scalar& backgroundColor = Scalar(0, 0, 0),
		const Scalar& forgroundColor = Scalar(255, 255, 255), bool gray = true);

	// Calculate a rect have same length and width and remains the center
	Rect adaptive_charrect_from_rect(const Rect& rect, int maxwidth, int maxheight, bool useExtendHeight = false);

	// 计算旋转矩形的最小外接矩形，并且对其四个顶点进行了验证，保证不超出图像范围
	EASYPR_API bool calcSafeRect(const RotatedRect& roi_rect, const Mat& src, Rect_<float>& safeBoundRect);
	EASYPR_API bool calcSafeRect(const RotatedRect &roi_rect, const int width, const int height, Rect_<float> &safeBoundRect);

	// uniform resize all the image to same size for the next process
	Mat uniformResize(const Mat &result, float& scale);

	// uniform resize all the plates to same size for the next process
	EASYPR_API Mat uniformResizePlates(const Mat &result, float& scale);

	// show detect results
	void showDectectResults(const Mat& img, const std::vector<CPlate> &plateVec, size_t num);

	// show the results
	Mat showResult(const Mat &result, int img_index = 0);

	// enlarge the char rect
	Rect rectEnlarge(const Rect& src, const int mat_width, const int mat_height);
	Rect rectFit(const Rect &src, const int mat_width, const int mat_height);

	// write images to temp folder
	EASYPR_API void writeTempImage(const Mat& outImg, const string path, int index = 0);

	// remove small hor lines in the plate
	bool judegMDOratio2(const Mat &image, const Rect &rect, std::vector<Point> &contour, Mat &result, const float thresh = 1.f,
		bool useExtendHeight = false);

	

	//! non-maximum surpresion for 1d array
	template<typename T>
	void NMSfor1D(const vector<T>& arr, vector<int>& index)
	{
		// prepare
		int size = (int)arr.size();
		index.resize(size);
		for (int j = 0; j < size; j++)
			index.at(j) = 0;

		// nms
		int i = 1;
		while (i < size - 1)
		{
			if (arr.at(i) > arr.at(i + 1))
			{
				if (arr.at(i) >= arr.at(i - 1))
					index.at(i) = 1;
			}
			else
			{
				while (i < size - 1 && arr.at(i) <= arr.at(i + 1))
					i = i + 1;
				if (i < size - 1)
					index.at(i) = 1;
			}
			i = i + 2;
		}
	}

} /*! \namespace easypr*/

#endif  // EASYPR_CORE_COREFUNC_H_