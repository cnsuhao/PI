#ifndef EASYPR_TRAIN_SVMTRAIN_H_
#define EASYPR_TRAIN_SVMTRAIN_H_

#include "train/train.h"
#include <vector>
#include "config.h"
#include "core/feature.h"
#include "basedef.h"
namespace easypr
{
	class EASYPR_API SvmTrain : public ITrain
	{
	public:
		typedef struct
		{
			std::string file;	// 图片文件名(包含扩展名)
			SvmLabel label;		// 图片标签，分为有车牌和没车牌两种
		} TrainItem;

		// 构造函数
		SvmTrain(const char* plates_folder, const char* xml);
		// 训练得到svm模型
		virtual void train();
		// 测试得到的svm模型
		virtual void test();

	private:
		// 根据文件夹路径，得到图片，准备训练和测试数据
		void _prepare();
		// 根据训练图片，创建svm训练数据
		virtual cv::Ptr<cv::ml::TrainData> _tdata();

	private:
		cv::Ptr<cv::ml::SVM>	m_pSvm;					// SVM类对象指针，用于训练和测试svm模型
		const char*				m_plates_folder;		// 存放了很多待训练的矩形图块的文件夹路径
		const char*				m_svm_xml;				// 用于存放训练得到的模型
		std::vector<TrainItem>	m_train_file_list;		// 待训练的图片条目集合
		std::vector<TrainItem>	m_test_file_list;		// 待测试的图片条目集合
		svmCallback				pfnExtractFeature;		// 特征提取的回调接口
		bool					m_isPrepared = true;	// 是否已经准备好了训练数据
	};
}

#endif  // EASYPR_TRAIN_SVMTRAIN_H_
