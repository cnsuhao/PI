#ifndef __IJZ_LOG_H__
#define __IJZ_LOG_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

// 接口基类
class IJZLog
{
public:
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
	// 写图像
	virtual JZ_RESULT WriteImage(JZImageBuf* pImageBuf, const char* filename) = 0;
	// 处理图像
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
	// 释放图像数据
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage) = 0;
};

typedef JZ_RESULT(*DefGetBaseImageInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseBaseImageInterface)(IJZBaseImageProcess*& pAPI);

// 供外部调用的接口声明
struct JZBaseImageProcessAPI
{
	DefGetBaseImageInterface pfnGetInterface;
	DefReleaseBaseImageInterface pfnReleaseInterface;
};

#endif // !__IJZ_LOG_H__
