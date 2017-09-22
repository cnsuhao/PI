//////////////////////////////////////////////////////////////////////////
// Name:	    plate_recognize Header
// Version:		1.0
// Date:	    2014-09-28
// Author:	    liuruoze
// Copyright:   liuruoze
// Reference:	Mastering OpenCV with Practical Computer Vision Projects
// Reference:	CSDN Bloger taotao1233
// Desciption:
// Defines CPlateRecognize
//////////////////////////////////////////////////////////////////////////
#ifndef EASYPR_CORE_PLATERECOGNIZE_H_
#define EASYPR_CORE_PLATERECOGNIZE_H_

#include "core/plate_detect.h"
#include "core/chars_recognise.h"
#include "basedef.h"
/*! \namespace easypr
Namespace where all the C++ EasyPR functionality resides
*/
namespace easypr
{

	class EASYPR_API CPlateRecognize : public CPlateDetect, public CCharsRecognise
	{
	public:
		// 构造函数
		CPlateRecognize();
		// 车牌识别，包括两个部分：车牌检测+字符识别
		int plateRecognize(const Mat& src, std::vector<CPlate> &plateVec, int img_index = 0);
		int plateRecognize(const Mat& src, std::vector<std::string> &licenseVec);
		// 参数设置/获取函数
		inline void setLifemode(bool param) { CPlateDetect::setPDLifemode(param); }
		inline void setDetectType(int param) { CPlateDetect::setDetectType(param); }

		inline void setResultShow(bool param) { m_showResult = param; }
		inline bool getResultShow() const { return m_showResult; }
		inline void setDetectShow(bool param) { CPlateDetect::setDetectShow(param); }
		inline void setDebug(bool param) { setResultShow(param); }
		// 读取SVM车牌区域识别模型
		void LoadSVM(std::string path);
		// 读取字符识别ann模型
		void LoadANN(std::string path);
		// 读取字符识别ann模型
		void LoadChineseANN(std::string path);

		//v1.6 added
		void LoadGrayChANN(std::string path);
		void LoadChineseMapping(std::string path);

	private:
		// show the detect and recognition result image
		bool m_showResult;
		DISABLE_ASSIGN_AND_COPY(CPlateRecognize);
	};

} /* \namespace easypr  */

#endif  // EASYPR_CORE_PLATERECOGNIZE_H_