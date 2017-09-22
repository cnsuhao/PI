#ifndef EASYPR_CORE_CHARSSEGMENT_H_
#define EASYPR_CORE_CHARSSEGMENT_H_

#include "opencv2/opencv.hpp"
#include "config.h"
#include "basedef.h"
using namespace cv;
using namespace std;

namespace easypr
{

	class EASYPR_API CCharsSegment
	{
	public:
		// 构造函数
		CCharsSegment();
		// 分割字符
		int charsSegment(Mat input, std::vector<Mat>& resultVec, Color color = BLUE);
		//! using methods to segment chars in plate
		int charsSegmentUsingOSTU(Mat input, std::vector<Mat>& resultVec, std::vector<Mat>& grayChars, Color color = BLUE);
		int charsSegmentUsingMSER(Mat input, vector<Mat>& resultVec, vector<Mat>& grayChars, Color color = BLUE);
		//! using project 
		int projectSegment(const Mat& input, Color color, vector<int>& out_indexs);

		// 参数设置/获取函数
		inline void setLiuDingSize(int param) { m_LiuDingSize = param; }
		inline void setColorThreshold(int param) { m_ColorThreshold = param; }

		inline void setBluePercent(float param) { m_BluePercent = param; }
		inline float getBluePercent() const { return m_BluePercent; }

		inline void setWhitePercent(float param) { m_WhitePercent = param; }
		inline float getWhitePercent() const { return m_WhitePercent; }

		inline void setDebug(int param) { m_debug = param; }
		inline int getDebug() { return m_debug; }

	private:
		// 判断字符块是否满足尺寸大小
		bool _verifyCharSizes(Mat r);
		// 判断字符矩形尺寸大小
		bool _verifyCharRectSizes(Rect r);
		// find the best chinese binaranzation method，中文字符阈值二值化
		void _judgeChinese(Mat in, Mat& out, Color plateType);
		void _judgeChineseGray(Mat in, Mat& out, Color plateType);
		// 滑动窗口中文字符阈值二值化
		bool _slideChineseWindow(Mat& image, Rect mr, Mat& newRoi, Color plateType, float slideLengthRatio, bool useAdapThreshold);
		bool _slideChineseGrayWindow(const Mat& image, Rect& mr, Mat& newRoi, Color plateType, float slideLengthRatio);
		// 将字符调整为指定大小20*20
		Mat _preprocessChar(Mat in);
		// 通过特殊字符块位置，寻找中文字符块
		Rect _GetChineseRect(const Rect rectSpe);
		// 寻找特殊字符块(简称后面的英文字符)
		int _GetSpecificRect(const std::vector<Rect>& vecRect);
		// 移除城市字符左边的矩形块，从城市字符右边开始共选择6个矩形块(包括城市字符)
		int _RebuildRect(const std::vector<Rect>& vecRect, std::vector<Rect>& outRect,
			int specIndex);
		int _getNearestIndex(Point center, const vector<Point>& groundCenters);

	private:
		int		m_LiuDingSize;			// 柳钉大小，默认为7
		int		m_theMatWidth;			// 车牌宽度，默认为136
		int		m_ColorThreshold;		// 颜色阈值，默认为150
		float	m_BluePercent;			// 蓝色百分比，默认为30%
		float	m_WhitePercent;			// 白色百分比，默认为10%
		int		m_debug;				// 是否进行调试，显示中间图像
	};

}

#endif  // EASYPR_CORE_CHARSSEGMENT_H_