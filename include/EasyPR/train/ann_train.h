#ifndef EASYPR_TRAIN_ANNTRAIN_H_
#define EASYPR_TRAIN_ANNTRAIN_H_

#include "train/train.h"
#include "util/kv.h"
#include <memory>
#include "basedef.h"
namespace easypr
{
	class EASYPR_API AnnTrain : public ITrain
	{
	public:
		// 构造函数
		explicit AnnTrain(const char* chars_folder, const char* xml);
		// 训练得到ann模型
		virtual void train();
		// 测试得到的ann模型
		virtual void test();

	private:
		// 生成输入样本点数据和输出样本点数据，即得到TrainData对象，用于神经网络训练
		virtual cv::Ptr<cv::ml::TrainData> _tdata();
		// 对给定字符图像进行几何变换(平移/旋转)，从而人工合成得到新的图像，用于训练
		cv::Mat _getSyntheticImage(const cv::Mat& image);
		// 生成输入样本点数据和输出样本点数据，即得到TrainData对象，用于神经网络训练，number_for_count表示每个字符至少要多少个图像，如果不足，则随机用之前的图像填充
		cv::Ptr<cv::ml::TrainData> _sdata(size_t number_for_count = 100);
		// 识别输入的中文字符图片(仅适用于中文字符图片)
		std::pair<std::string, std::string> _identifyChinese(cv::Mat input);
		// 识别输入的字符图片(适用于中文、字母、数字)
		std::pair<std::string, std::string> _identify(cv::Mat input);


	private:
		cv::Ptr<cv::ml::ANN_MLP>	m_ann;				// ANN_MLP类对象指针，用于训练和测试ann模型
		const char*					m_ann_xml;			// 用于存放训练得到的模型
		const char*					m_chars_folder;		// 存放了很多待训练的矩形图块的文件夹路径
		std::shared_ptr<Kv>			m_kv;
		int							m_type;				// 0-表示训练中文字符、数字和字母共65个字符；1-表示训练中文字符
	};
}

#endif  // EASYPR_TRAIN_ANNTRAIN_H_