#ifndef __JZ_BASEIMAGEPROCESS_H__
#define __JZ_BASEIMAGEPROCESS_H__

#include "IJZBaseImageProcess.h"

class JZBaseImageProcess : public IJZBaseImageProcess
{
public:
	// 构造函数
	JZBaseImageProcess();
	// 析构函数
	~JZBaseImageProcess();
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// 写图像
	JZ_RESULT WriteImage(JZImageBuf* pImageBuf, const char* filename);
	// 释放图像数据
	JZ_RESULT ReleaseImage(JZImageBuf* pImage);
	// 处理图像
	JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);

private:
	// 图像形态学处理
	JZ_RESULT _ImageMorphology(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
	// 图像平滑处理
	JZ_RESULT _ImageSmooth(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};
#endif // !__JZ_BASEIMAGEPROCESS_H__

