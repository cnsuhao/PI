#ifndef EASYPR_TRAIN_TRAIN_H_
#define EASYPR_TRAIN_TRAIN_H_

#include <opencv2/opencv.hpp>

namespace easypr
{
	class ITrain
	{
	public:
		ITrain();

		virtual ~ITrain();
		// 训练得到模型
		virtual void train() = 0;
		// 测试得到的模型
		virtual void test() = 0;

	private:
		// 根据训练图片，创建训练数据
		virtual cv::Ptr<cv::ml::TrainData> _tdata() = 0;
	};
}

#endif  // EASYPR_TRAIN_TRAIN_H_