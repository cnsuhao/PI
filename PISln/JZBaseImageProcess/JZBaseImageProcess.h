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
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// 图像平滑(模糊)
	virtual JZ_RESULT BlurImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// 释放图像数据
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage);

};
#endif // !__JZ_BASEIMAGEPROCESS_H__

