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
	// 写图像
	JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  JZImageBuf, JZCommonParam* param);
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// 释放图像数据
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage);

};
#endif // !__JZ_BASEIMAGEPROCESS_H__

