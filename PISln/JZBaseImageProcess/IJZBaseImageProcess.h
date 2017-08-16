#ifndef __IJZBASEIMAGEPROCESS_H__
#define __IJZBASEIMAGEPROCESS_H__

#include <opencv2\opencv.hpp>
using namespace std;

struct JZCommonParam
{

};

enum JZ_RESULT
{
	JZ_OK,
	JZ_INVAILD_PARAM,
};

class IJZBaseImageProcess
{
public:
	IJZBaseImageProcess();
	~IJZBaseImageProcess();
	virtual JZ_RESULT ProcessImage(Mat* src, Mat*  des, JZCommonParam* param);

private:
	Mat* pSrcImg;
	Mat* pDesImg;
	JZCommonParam* pParam;
};

IJZBaseImageProcess::IJZBaseImageProcess()
{
}

IJZBaseImageProcess::~IJZBaseImageProcess()
{
}

#endif
