#ifndef EASYPR_CORE_PLATEJUDGE_H_
#define EASYPR_CORE_PLATEJUDGE_H_

#include "core/plate.hpp"
#include "core/feature.h"
#include "basedef.h"
namespace easypr
{

	class EASYPR_API PlateJudge
	{
	public:
		// 构造函数
		PlateJudge();
		// 获取单例对象
		static PlateJudge* instance();
		// 根据xml文件路径，读取SVM模型
		void LoadModel(std::string path);
		// 在SVM模型得分的基础上，加上非极大值抑制（NMS）来判断是否为车牌
		int plateJudgeUsingNMS(const std::vector<CPlate>&, std::vector<CPlate>&, int maxPlates = 5);
		// 根据SVM模型判断是否为车牌
		int plateJudge(const Mat& plateMat);
		// 根据SVM模型判断是否为车牌
		int plateJudge(const std::vector<Mat> &inVec,
			std::vector<Mat> &resultVec);
		// 根据SVM模型判断是否为车牌
		int plateJudge(const std::vector<CPlate> &inVec,
			std::vector<CPlate> &resultVec);

	private:
		// 根据SVM模型，预测计算是车牌的得分，判断是否为车牌，0 is plate, -1 is not.
		int _plateSetScore(CPlate& plate);
		// 非极大值抑制
		void _NMS(std::vector<CPlate> &inVec, std::vector<CPlate> &resultVec, double overlap);
	private:
		cv::Ptr<ml::SVM> m_pSvm;			// SVM对象指针
		static PlateJudge* m_pInstance;		// 单例对象
		svmCallback pfnExtractFeature;		// 特征提取的回调接口



	};
}

#endif  // EASYPR_CORE_PLATEJUDGE_H_