#ifndef __IJZ_BASEIMAGEPROCESS_H__
#define __IJZ_BASEIMAGEPROCESS_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

// 接口基类
class IJZBaseImageProcess
{
public:
	// 写图像
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
	// 图像平滑(模糊)
	virtual JZ_RESULT BlurImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	// 释放图像数据
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage) = 0;
};

typedef JZ_RESULT(*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);

// 供外部调用的接口声明
struct JZBaseImageProcessAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};


#endif
