//////////////////////////////////////////////////////////////////////////
// Name:	    plate_locate Header
// Version:		1.2
// Date:	    2014-09-19
// MDate:		2014-09-29
// MDate:	    2015-03-13
// Author:	    liuruoze
// Copyright:   liuruoze
// Reference:	Mastering OpenCV with Practical Computer Vision Projects
// Reference:	CSDN Bloger taotao1233
// Desciption:
// Defines CPlateLocate
//////////////////////////////////////////////////////////////////////////
#ifndef EASYPR_CORE_PLATELOCATE_H_
#define EASYPR_CORE_PLATELOCATE_H_

#include "core/plate.hpp"
#include "basedef.h"
/*! \namespace easypr
	Namespace where all the C++ EasyPR functionality resides
*/

using namespace std;

namespace easypr
{

	class EASYPR_API CPlateLocate
	{
	public:
		// 构造函数
		CPlateLocate();

		// 设置参数
		void setLifemode(bool param);

		// 获取/设置各个参数的值
		inline void setGaussianBlurSize(int param) { m_GaussianBlurSize = param; }
		inline int getGaussianBlurSize() const { return m_GaussianBlurSize; }

		inline void setMorphSizeWidth(int param) { m_MorphSizeWidth = param; }
		inline int getMorphSizeWidth() const { return m_MorphSizeWidth; }

		inline void setMorphSizeHeight(int param) { m_MorphSizeHeight = param; }
		inline int getMorphSizeHeight() const { return m_MorphSizeHeight; }

		inline void setVerifyError(float param) { m_error = param; }
		inline float getVerifyError() const { return m_error; }

		inline void setVerifyAspect(float param) { m_aspect = param; }
		inline float getVerifyAspect() const { return m_aspect; }

		inline void setVerifyMin(int param) { m_verifyMin = param; }
		inline void setVerifyMax(int param) { m_verifyMax = param; }

		inline void setJudgeAngle(int param) { m_angle = param; }

		inline void setDebug(bool param) { m_debug = param; }
		inline bool getDebug() { return m_debug; }

		// 颜色定位车牌区域
		int plateColorLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
		// Sobel定位车牌区域
		int plateSobelLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
		// Mser文字定位车牌区域
		int plateMserLocate(Mat src, std::vector<CPlate>& candPlates, int index = 0);
		// 综合车牌区域定位
		int plateLocate(Mat, std::vector<Mat>&, int = 0);
		// 综合车牌区域定位
		int plateLocate(Mat, std::vector<CPlate>&, int = 0);

	private:
		// 去噪、Sobel边缘提取、形态学处理
		int _sobelOper(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);
		// 去噪、Sobel边缘提取、形态学处理
		int _sobelOperT(const Mat& in, Mat& out, int blurSize, int morphW, int morphH);
		// 尺寸判断（根据车牌面积和宽高比来进行尺寸判断）
		bool _verifySizes(RotatedRect mr);

		// 在源图像中搜索矩形车牌区域
		int _sobelFrtSearch(const Mat& src, std::vector<Rect_<float>>& outRects);
		// 在得到的矩形图块中二次搜索车牌区域，refpoint是bound图像再原图像中左上角顶点的坐标
		int _sobelSecSearch(Mat& bound, Point2f refpoint,
			std::vector<RotatedRect>& outRects);
		// 在矩形块中再次进行搜索，这里是经过扩充后的矩形块
		int _sobelSecSearchPart(Mat& bound, Point2f refpoint,
			std::vector<RotatedRect>& outRects);

		// 通过颜色搜索来定位矩形车牌区域
		int _colorSearch(const Mat& src, const Color r, Mat& out,
			std::vector<RotatedRect>& outRects);
		// 通过文字定位来确定矩形车牌区域
		int _mserSearch(const Mat &src, vector<Mat>& out,
			vector<vector<CPlate>>& out_plateVec, bool usePlateMser, vector<vector<RotatedRect>>& out_plateRRect,
			int img_index = 0, bool showDebug = false);

		// 将图形块扩大后，进行旋转
		bool _rotation(Mat& in, Mat& out, const Size rect_size, const Point2f center,
			const double angle);
		// 计算斜率，判断车牌是否偏斜了，slope是斜率
		bool _isdeflection(const Mat& in, const double angle, double& slope);
		// 仿射变换，扭正车牌
		void _affine(const Mat& in, Mat& out, const double slope);
		// 删除无效区域，比如柳钉， 左右边界无效区域
		void _deleteNotArea(Mat &inmat, Color color = UNKNOWN);
		// 抗扭斜处理
		int _deskew(const Mat& src, const Mat& src_b, std::vector<RotatedRect>& inRects, std::vector<CPlate>& outPlates,
			bool useDeteleArea = true, Color color = UNKNOWN);

	protected:
		int		m_GaussianBlurSize;		// 高斯模糊元素大小
		int		m_MorphSizeWidth;		// 形态学操作，模板宽
		int		m_MorphSizeHeight;		// 形态学操作，模板高

		float	m_error;				// 宽高比偏差率
		float	m_aspect;				// 一般车牌的宽高比
		int		m_verifyMin;			// 最小面积系数
		int		m_verifyMax;			// 最大面积系数
		int		m_angle;				// 用于车牌判断的倾斜角度

		bool	m_debug;				// 是否开启调试，即图像显示、存储等		
	};

} /*! \namespace easypr*/

#endif  // EASYPR_CORE_PLATELOCATE_H_