#ifndef EASYPR_TRAIN_ANNCHTRAIN_H_
#define EASYPR_TRAIN_ANNCHTRAIN_H_

#include "train/train.h"
#include "util/kv.h"
#include "core/feature.h"
#include <memory>
#include "basedef.h"
namespace easypr
{

	class EASYPR_API AnnChTrain : public ITrain
	{
	public:
		// 构造函数
		explicit AnnChTrain(const char* chars_folder, const char* xml);
		// 用原始数据或者（原始数据+合成数据）进行训练得到ann模型
		void trainVal(size_t number_for_count = 100);
		// 训练得到ann模型
		virtual void train();
		// 测试得到的ann模型
		virtual void test();
		// 设置需要的字符图像的个数
		inline void setNumberForCount(int n) { m_number_for_count = n; }

	private:
		// 生成输入样本点数据和输出样本点数据，即得到TrainData对象，用于神经网络训练
		virtual cv::Ptr<cv::ml::TrainData> _tdata();
		// 识别灰度中文字符图像
		std::pair<std::string, std::string> _identifyGrayChinese(cv::Mat input);


	private:
		cv::Ptr<cv::ml::ANN_MLP>	m_ann;					// ANN_MLP类对象指针，用于训练和测试ann模型
		const char*					m_ann_xml;				// 用于存放训练得到的模型
		const char*					m_chars_folder;			// 存放了很多待训练的矩形图块的文件夹路径

		std::shared_ptr<Kv>			m_kv;
		int							m_type;					// 0-表示训练中文字符、数字和字母共65个字符；1-表示训练中文字符
		int							m_number_for_count;		// 表示每个字符至少要多少个图像，如果不足，则随机用之前的图像填充
		annCallback					pfnExtractFeature;		// 特征提取的回调接口
	};
}

#endif  // EASYPR_TRAIN_ANNCHTRAIN_H_