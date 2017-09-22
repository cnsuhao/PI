#ifndef EASYPR_CORE_PLATEDETECT_H_
#define EASYPR_CORE_PLATEDETECT_H_

#include "core/plate_locate.h"
#include "core/plate_judge.h"
#include "basedef.h"
namespace easypr
{

	class EASYPR_API CPlateDetect
	{
	public:
		// 构造函数
		CPlateDetect();
		// 析构函数
		~CPlateDetect();

		/** @brief Plate detect in an image.

		The function detects plate in an image. It can use sobel, color, and character method or the combinations of them.

		@param src Source image.
		@param resultVec Destination vector of CPlate.
		@param type Detect type. (eg. PR_DETECT_SOBEL + PR_DETECT_COLOR)
		@param showDetectArea
		@param index
		*/
		int plateDetect(Mat src, std::vector<CPlate> &resultVec, int type,
			bool showDetectArea, int img_index = 0);


		/** @brief Plate detect in an image.

		The function detects plate in an image. It can use sobel, color, and character method or the combinations of them.
		Use default m_type, it can use setDetectType() to set it;

		@param src Source image.
		@param resultVec Destination vector of CPlate.
		@param index
		*/
		int plateDetect(Mat src, std::vector<CPlate> &resultVec, int img_index = 0);

		// 根据模型的xml文件路径，读取SVM模型
		void LoadSVM(std::string s);

		inline void setPDLifemode(bool param) { m_plateLocate->setLifemode(param); }

		inline void setPDDebug(bool param)
		{
			m_plateLocate->setDebug(param);
			setDetectShow(param);
		}

		inline bool getPDDebug() { return m_plateLocate->getDebug(); }

		inline void setDetectType(int param) { m_type = param; }

		inline void setGaussianBlurSize(int param) { m_plateLocate->setGaussianBlurSize(param); }

		inline int getGaussianBlurSize() const { return m_plateLocate->getGaussianBlurSize(); }

		inline void setMorphSizeWidth(int param) { m_plateLocate->setMorphSizeWidth(param); }

		inline int getMorphSizeWidth() const { return m_plateLocate->getMorphSizeWidth(); }

		inline void setMorphSizeHeight(int param) { m_plateLocate->setMorphSizeHeight(param); }

		inline int getMorphSizeHeight() const { return m_plateLocate->getMorphSizeHeight(); }

		inline void setVerifyError(float param) { m_plateLocate->setVerifyError(param); }

		inline float getVerifyError() const { return m_plateLocate->getVerifyError(); }

		inline void setVerifyAspect(float param) { m_plateLocate->setVerifyAspect(param); }

		inline float getVerifyAspect() const { return m_plateLocate->getVerifyAspect(); }

		inline void setVerifyMin(int param) { m_plateLocate->setVerifyMin(param); }

		inline void setVerifyMax(int param) { m_plateLocate->setVerifyMax(param); }

		inline void setJudgeAngle(int param) { m_plateLocate->setJudgeAngle(param); }

		inline void setMaxPlates(int param) { m_maxPlates = param; }

		inline int getMaxPlates() const { return m_maxPlates; }

		inline void setDetectShow(bool param) { m_showDetect = param; }
		inline bool getDetectShow() const { return m_showDetect; }

	private:
		static std::string m_pathSvm;		// SVM模型的xml文件路径
		int m_maxPlates;					// 最多车牌数量
		int m_type;							// 车牌检测方法类型，可以是颜色、sobel、文字定位方法中的一种，或者是它们的联合
		bool m_showDetect;					// 是否展示检测结果图像

		CPlateLocate* m_plateLocate;		// 车牌定位对象指针

	};

}

#endif  // EASYPR_CORE_PLATEDETECT_H_